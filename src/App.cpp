#include "App.h"
#include "AppManager.h"
#include <fcntl.h>
#include <igris/util/base64.h>
#include <igris/util/string.h>
#include <iostream>
#include <modes.h>
#include <nos/fprint.h>
#include <poll.h>
#include <pwd.h>
#include <shutdown.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

extern std::unique_ptr<AppManager> appManager;

// std::mutex App::ioMutex;
using namespace std::chrono_literals;

nos::trent LinkedFile::to_trent() const
{
    nos::trent tr;
    tr["path"] = path;
    tr["name"] = name;
    tr["editable"] = editable;
    return tr;
}

void App::set_user(const std::string &user)
{
    this->_username = user;
    if (!user.empty())
    {
        struct passwd *pw = getpwnam(user.c_str());
        if (pw)
        {
            this->_uid = pw->pw_uid;
            //    this->_gid = pw->pw_gid;
        }
    }
}

std::string App::status_string() const
{
    if (isStopped)
        return "stopped";
    else
        return "running";
}

std::string App::command() const
{
    std::string cmd;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        cmd += tokens[i];
        if (i + 1 < tokens.size())
            cmd += " ";
    }
    return cmd;
}

std::string App::token_list_as_string() const
{
    std::string cmd = "";
    cmd += "[";
    for (unsigned int i = 0; i < tokens.size(); ++i)
    {
        auto &arg = tokens[i];
        cmd += arg;
        if (i != tokens.size() - 1)
            cmd += ",";
    }
    cmd += "]";
    return cmd;
}

std::string GetStdoutFromCommand(std::string cmd)
{

    std::string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
        {
            if (fgets(buffer, max_buffer, stream) != NULL)
                data.append(buffer);
        }
        pclose(stream);
    }
    return data;
}

App::App(int task_index, const std::string &name, const std::string &cmd,
         RestartMode mode, const std::vector<LinkedFile> &linkeds,
         std::string user)
    : _linked_files(linkeds), task_index(task_index), _name(name)
{
    tokens = igris::split_cmdargs(cmd);
    _restartMode = mode;
    set_user(user);
}

std::string execute_and_read_output(const std::string &cmd)
{
    igris::subprocess proc;
    std::vector<std::string> args = igris::split(cmd);
    proc.exec(args[0], args, {});
    int fd = proc.output_fd();
    std::string fullout;

    if (fd < 0)
    {
        proc.wait();
        return fullout;
    }

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    while (!is_shutdown_requested())
    {
        int poll_result = poll(&pfd, 1, 100); // 100ms timeout

        if (poll_result < 0)
        {
            if (errno == EINTR)
                continue;
            break;
        }

        if (poll_result == 0)
        {
            // Timeout - check if process finished
            int status;
            pid_t result = waitpid(proc.pid(), &status, WNOHANG);
            if (result > 0)
                break;
            continue;
        }

        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL))
            break;

        if (pfd.revents & POLLIN)
        {
            char buf[1024];
            ssize_t n = ::read(fd, buf, sizeof(buf));
            if (n > 0)
                fullout.append(buf, static_cast<size_t>(n));
            else
                break;
        }
    }

    // If shutdown requested, kill the subprocess
    if (is_shutdown_requested())
    {
        proc.kill();
    }

    proc.wait();
    proc.close();
    return fullout;
}

void App::stop()
{
    nos::fprintln("[App::stop] Stopping '{}'...", name());

    if (systemd_bind != "")
    {
        std::string cmd = "/usr/bin/systemctl stop " + systemd_bind;
        nos::fprintln("[App::stop] Stop in systemctl mode: {}", cmd);
        std::string out = execute_and_read_output(cmd);
        nos::println(out);
        isStopped = true;
        systemd_pid = 0;
        nos::fprintln("[App::stop] '{}' systemd stop done", name());
        return;
    }

    if (isStopped && !_watcher_guard)
    {
        nos::fprintln("[App::stop] '{}' is already stopped", name());
        return;
    }

    if (_watcher_guard)
    {
        nos::fprintln("[App::stop] '{}' killing process pid={}...", name(), proc.pid());
        _attempts = 0;
        cancel_reading = true;
        proc.kill();
        nos::fprintln("[App::stop] '{}' process killed, joining watcher...", name());

        // Ждем завершения watcher thread
        if (_watcher_thread.joinable())
            _watcher_thread.join();
        nos::fprintln("[App::stop] '{}' watcher joined", name());
    }
}

void App::restart()
{
    nos::println("Restarting app: {}", name());
    stop();
    std::this_thread::sleep_for(0.5s);
    start();
}

void App::start()
{
    if (systemd_bind != "")
    {
        start_systemd();
        return;
    }

    if (isStopped)
    {
        restart_attempt_counter();
        isStopped = false;
        run();
    }
}

void App::start_systemd()
{
    std::string cmd = "/usr/bin/systemctl start " + systemd_bind;
    nos::fprintln("Start in systemctl mode: {}", cmd);
    restart_attempt_counter();
    std::string out = execute_and_read_output(cmd);
    nos::println(out);
    isStopped = false;
    _startTime = std::chrono::system_clock::now();
    systemd_pid = 0;
}

std::string App::get_journal_data(int lines_count)
{
    std::string path = "/tmp/journal_cadscasdcv.log";
    std::string out1 = execute_and_read_output(
        nos::format("/usr/bin/journalctl -u {} -n {}", name(), lines_count));
    nos::println(out1);
    // std::string out2 = execute_and_read_output(
    //     nos::format("/usr/bin/cat {}", path));
    // nos::println(out2);
    return out1;
}

std::vector<char *> App::tokens_for_execve(const std::vector<std::string> &args)
{
    std::vector<char *> res;
    for (auto &arg : args)
    {
        res.push_back(const_cast<char *>(arg.c_str()));
    }
    res.push_back(nullptr);
    return res;
}

std::vector<std::string> App::envp_base_for_execve()
{
    std::vector<std::string> res;
    for (auto &arg : _env)
    {
        std::string str = arg.first + "=" + arg.second;
        res.push_back(str);
    }
    return res;
}

std::vector<char *> App::envp_for_execve(const std::vector<std::string> &args)
{
    std::vector<char *> res;
    for (auto &arg : args)
    {
        res.push_back(const_cast<char *>(arg.c_str()));
    }
    res.push_back(nullptr);
    return res;
}

void App::set_systemd_bind(const std::string &service)
{
    this->systemd_bind = service;
}

// Call only in a separate thread
void App::appFork()
{
    nos::fprintln("[appFork] '{}' ENTER", name());
    _exitStatus = 0;
    increment_attempt_counter();

    _startTime = std::chrono::system_clock::now();

    auto envp_base = envp_base_for_execve();
    auto envp = envp_for_execve(envp_base);
    auto args = tokens_for_execve(tokens);

    nos::fprintln("[appFork] '{}' calling proc.exec()...", name());
    proc.exec(tokens[0].data(), args, envp);
    nos::fprintln("[appFork] '{}' proc.exec() returned, pid={}", name(), proc.pid());
    int fd = proc.output_fd();
    nos::fprintln("[appFork] '{}' output_fd={}", name(), fd);

    std::vector<uint8_t> buffer;
    char buf[1024];
    buffer.reserve(2048);

    // Если fd невалидный, просто ждем завершения процесса
    if (fd < 0)
    {
        nos::println("No valid output fd, waiting for process to finish");
        proc.wait();
        cancel_reading = false;
        nos::println("Finish subprocess:", name());
        return;
    }

    // Используем poll для неблокирующего ожидания данных
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    nos::fprintln("[appFork] '{}' entering poll loop", name());
    int loop_count = 0;
    while (true)
    {
        loop_count++;
        if (loop_count % 100 == 1)
            nos::fprintln("[appFork] '{}' poll loop iteration {}", name(), loop_count);

        if (cancel_reading)
        {
            nos::fprintln("[appFork] '{}' cancel_reading=true, breaking (loop {})", name(), loop_count);
            break;
        }

        // poll с таймаутом 100ms
        int poll_result = poll(&pfd, 1, 100);

        if (poll_result < 0)
        {
            if (errno == EINTR)
            {
                nos::fprintln("[appFork] '{}' poll EINTR (loop {})", name(), loop_count);
                continue;
            }
            nos::fprintln("[appFork] '{}' poll error (loop {})", name(), loop_count);
            perror("poll");
            break;
        }

        if (poll_result == 0)
        {
            // Таймаут - проверяем жив ли процесс
            int status;
            pid_t result = waitpid(proc.pid(), &status, WNOHANG);
            if (result > 0)
            {
                nos::fprintln("[appFork] '{}' waitpid returned {} (loop {})", name(), result, loop_count);
                nos::println("Process finished with status:",
                             WEXITSTATUS(status));
                break;
            }
            else if (result < 0)
            {
                // ECHILD - process already reaped by SIGCHLD handler
                nos::fprintln("[appFork] '{}' waitpid returned -1 (ECHILD), process already reaped (loop {})", name(), loop_count);
                break;
            }
            // result == 0 means process still running, continue polling
            continue;
        }

        // Есть данные для чтения или ошибка
        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL))
        {
            nos::fprintln("[appFork] '{}' POLLERR/HUP/NVAL revents=0x{:x} (loop {})",
                         name(), pfd.revents, loop_count);
            std::cout.flush();
            // PTY закрыт или ошибка - процесс вероятно завершился
            int status;
            nos::fprintln("[appFork] '{}' calling waitpid({}, WNOHANG)...", name(), proc.pid());
            std::cout.flush();
            pid_t result = waitpid(proc.pid(), &status, WNOHANG);
            nos::fprintln("[appFork] '{}' waitpid returned {}, errno={}", name(), result, errno);
            std::cout.flush();
            if (result > 0)
            {
                nos::fprintln("[appFork] '{}' process finished with status: {}", name(), WEXITSTATUS(status));
            }
            else if (result < 0)
            {
                nos::fprintln("[appFork] '{}' process already reaped (ECHILD)", name());
            }
            else
            {
                nos::fprintln("[appFork] '{}' waitpid returned 0 (process still running?)", name());
            }
            std::cout.flush();
            nos::fprintln("[appFork] '{}' breaking from poll loop", name());
            std::cout.flush();
            break;
        }

        if (pfd.revents & POLLIN)
        {
            int n = read(fd, buf, sizeof(buf));
            if (n > 0)
            {
                logdata_append(buf, n);
                buffer.clear();
                buffer.push_back((uint8_t)task_index);
                buffer.push_back((uint8_t)name().size());
                buffer.insert(buffer.end(), _name.begin(), _name.end());
                uint16_t len = n;
                buffer.push_back((uint8_t)(len >> 8));
                buffer.push_back((uint8_t)(len & 0xFF));
                buffer.insert(buffer.end(), buf, buf + n);
                if (appManager)
                    appManager->send_spam(buffer);
            }
            else if (n == 0)
            {
                nos::println("EOF from subprocess:", name());
                break;
            }
            else
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                    continue;

                // EIO означает что slave PTY закрыт
                if (errno == EIO)
                {
                    int status;
                    pid_t result = waitpid(proc.pid(), &status, WNOHANG);
                    if (result > 0)
                    {
                        nos::println("Process finished with status:",
                                     WEXITSTATUS(status));
                    }
                    break;
                }
                perror("read");
                break;
            }
        }
    }

    // Убедимся, что процесс полностью завершился (non-blocking чтобы избежать гонки с SIGCHLD)
    nos::fprintln("[appFork] '{}' exited poll loop, waiting for pid={} (non-blocking)...", name(), proc.pid());
    std::cout.flush();

    // Используем non-blocking waitpid вместо blocking proc.wait()
    // потому что SIGCHLD handler может забрать процесс раньше
    int wait_loops = 0;
    while (true)
    {
        int status;
        pid_t result = waitpid(proc.pid(), &status, WNOHANG);
        if (result > 0)
        {
            nos::fprintln("[appFork] '{}' waitpid returned {} (exited)", name(), result);
            break;
        }
        else if (result < 0)
        {
            // ECHILD - процесс уже забран SIGCHLD хендлером
            nos::fprintln("[appFork] '{}' waitpid returned -1 (already reaped by SIGCHLD)", name());
            break;
        }
        // result == 0 - процесс ещё работает
        if (++wait_loops > 50) // 5 секунд максимум
        {
            nos::fprintln("[appFork] '{}' wait timeout, giving up", name());
            break;
        }
        std::this_thread::sleep_for(100ms);
    }

    nos::fprintln("[appFork] '{}' wait done", name());
    std::cout.flush();

    cancel_reading = false;
    nos::println("Finish subprocess:", name());
}

bool App::stopped() const
{
    return isStopped;
}

App::RestartMode App::restartMode() const
{
    return _restartMode;
}

int App::pid() const
{
    if (!systemd_bind.empty())
    {
        return systemd_pid;
    }

    return proc.pid();
}

const std::string &App::name() const
{
    return _name;
}

// retrurn log pathes
std::vector<std::string> App::logPaths() const
{
    return {};
}

int64_t App::uptime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now() - _startTime)
        .count();
}

void App::restart_attempt_counter()
{
    _attempts = _attempts_initializer;
}

void App::increment_attempt_counter()
{
    _attempts--;
    assert(_attempts >= 0);
}

bool App::need_to_another_attempt() const
{
    return restartMode() == RestartMode::ALWAYS && _attempts != 0;
}

void App::watchFunc()
{
    nos::fprintln("[watchFunc] '{}' started, thread ID: {}", name(), std::this_thread::get_id());
    while (1)
    {
        std::this_thread::sleep_for(10ms);
        if (cancel_reading)
        {
            nos::fprintln("[watchFunc] '{}' cancel_reading detected, breaking", name());
            break;
        }

        nos::println("appFork", name());
        appFork();

        if (!need_to_another_attempt())
        {
            nos::fprintln("[watchFunc] '{}' no more attempts, breaking", name());
            break;
        }
    }

    nos::fprintln("[watchFunc] '{}' exiting, setting state...", name());
    isStopped = true;
    cancel_reading = false;
    _watcher_guard = false;
    proc.invalidate();
    nos::fprintln("[watchFunc] '{}' done", name());
}

void App::run()
{
    nos::fprintln("[App::run] '{}' called from thread {}", name(), std::this_thread::get_id());
    if (!_watcher_guard)
    {
        _watcher_guard = true;
        if (_watcher_thread.joinable())
        {
            nos::fprintln("[App::run] '{}' joining previous watcher thread...", name());
            _watcher_thread.join();
            nos::fprintln("[App::run] '{}' previous watcher thread joined", name());
        }
        nos::fprintln("[App::run] '{}' spawning watcher thread...", name());
        _watcher_thread = std::thread(&App::watchFunc, this);
        nos::fprintln("[App::run] '{}' watcher thread spawned", name());
    }
    else
        nos::fprintln(
            "[App::run] Can't start app '{}' because its watcher is already running",
            name());
}

std::queue<int> &App::errors()
{
    return _errors;
}

void App::logdata_clear()
{
    _stdout_record.clear();
    _stdout_record.reserve(1024 * 1024);
}

void App::logdata_append(const char *data, size_t size)
{
    if (PRINT_LOGS)
        nos::print(nos::buffer(data, size));
    logstream_subject.get_subscriber().on_next(std::string{data, size});
    _stdout_record.append(data, size);
}

size_t App::logdata_size() const
{
    return _stdout_record.size();
}

int64_t App::logdata_read(char *buf, size_t size, size_t offset)
{
    if (offset >= _stdout_record.size())
        return 0;
    auto fullsize = logdata_size();
    if (offset + size > fullsize)
        size = fullsize - offset;
    memcpy(buf, _stdout_record.data() + offset, size);
    return size;
}

const std::string &App::show_stdout() const
{
    return _stdout_record;
}

void App::on_child_finished()
{
    cancel_reading = true;
}

void App::set_environment_variables(
    const std::unordered_map<std::string, std::string> &env)
{
    _env = env;
}

bool App::is_systemctl_process()
{
    return systemd_bind != "";
}

void App::set_pid(int p)
{
    systemd_pid = p;
}

void App::setCommand(const std::string &cmd)
{
    tokens = igris::split_cmdargs(cmd);
}

void App::setRestartMode(RestartMode mode)
{
    _restartMode = mode;
}

void App::setName(const std::string &newName)
{
    _name = newName;
}

nos::trent App::toTrent() const
{
    nos::trent tr;
    tr["name"] = _name;
    tr["command"] = command();
    tr["restart"] = (_restartMode == ALWAYS) ? "always" : "once";
    if (!_username.empty())
    {
        tr["user"] = _username;
    }
    if (!systemd_bind.empty())
    {
        tr["systemd_bind"] = systemd_bind;
    }
    if (!_env.empty())
    {
        for (const auto &kv : _env)
        {
            tr["env"][kv.first] = kv.second;
        }
    }
    if (!_linked_files.empty())
    {
        for (size_t i = 0; i < _linked_files.size(); i++)
        {
            tr["files"][(int)i] = _linked_files[i].to_trent();
        }
    }
    return tr;
}

void AppManager::update_systemctl_projects_status()
{
    nos::fprintln("[systemd_updater] Thread started, ID: {}", std::this_thread::get_id());
    while (!is_shutdown_requested())
    {
        for (std::shared_ptr<App> p : applications())
        {
            if (is_shutdown_requested())
                break;

            if (p->is_systemctl_process())
            {
                auto name = p->systemd_bind;

                if (p->pid() == 0)
                {
                    auto out = execute_and_read_output(nos::format(
                        "/usr/bin/systemctl show --property MainPID --value {}",
                        name));
                    auto trimmed = nos::trim(out);
                    if (!trimmed.empty() && std::isdigit(static_cast<unsigned char>(trimmed[0])))
                    {
                        try {
                            int pid = std::stoi(trimmed);
                            p->set_pid(pid);
                        } catch (const std::exception& e) {
                            nos::fprintln("[systemd_updater] Failed to parse PID for '{}': '{}'", name, trimmed);
                        }
                    }
                }

                {
                    auto out = execute_and_read_output(
                        nos::format("/usr/bin/systemctl is-active {}", name));
                    auto trimmed = nos::trim(out);
                    bool is_active = trimmed == "active";
                    p->isStopped = !is_active;
                }
            }
        }

        // Use wait_for_shutdown instead of plain sleep for faster response
        if (wait_for_shutdown_ms(300))
            break;
    }
    nos::println("Systemd updater thread stopped");
}
