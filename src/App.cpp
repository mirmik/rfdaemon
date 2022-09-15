#include "App.h"
#include "AppManager.h"
#include <igris/util/base64.h>
#include <igris/util/string.h>
#include <iostream>
#include <nos/fprint.h>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

extern AppManager *appManager;

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
    std::string cmd = "";
    for (auto &arg : tokens)
    {
        cmd += arg + " ";
    }
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
    tokens = igris::split(cmd);
    _restartMode = mode;
    set_user(user);
}

void App::stop()
{
    if (!isStopped)
    {
        _attempts = 0;
        kill(_pid, SIGKILL);
    }
}

void App::restart()
{
    stop();
    std::this_thread::sleep_for(0.5s);
    start();
}

void App::start()
{
    if (isStopped)
    {
        restart_attempt_counter();
        run();
    }
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

// Call only in a separate thread
pid_t App::appFork()
{
    _exitStatus = 0;
    increment_attempt_counter();

    int wr[2];
    pipe(wr);
    _startTime = std::chrono::system_clock::now();

    pid_t pid = fork();
    if (pid == 0)
    {
        close(wr[0]);
        dup2(wr[1], STDOUT_FILENO);
        close(wr[1]);
        nos::fprintln("Execv app : {}", tokens);

        // Timeout for main thread can connect to the pipe
        std::this_thread::sleep_for(100ms);
        exit(execv(tokens[0].data(), tokens_for_execve(tokens).data()));
    }
    else if (pid > 0)
    {
        close(wr[1]);
        isStopped = false;
        _pid = pid;
        ssize_t n;

        std::vector<uint8_t> buffer;
        char buf[1024];
        buffer.reserve(2048);

        nos::osutil::nonblock(wr[0], true);

        while (true)
        {
            n = read(wr[0], buf, sizeof(buf));
            if (n > 0)
            {
                _stdout_record.append(std::string(buf, n));
                buffer.clear();
                buffer.push_back((uint8_t)task_index);
                buffer.push_back((uint8_t)name().size());
                buffer.insert(buffer.end(), _name.begin(), _name.end());
                uint16_t len = n;
                buffer.push_back((uint8_t)(len >> 8));
                buffer.push_back((uint8_t)(len & 0xFF));
                buffer.insert(buffer.end(), buf, buf + n);
                appManager->send_spam(buffer);
            }

            if (cancel_reading)
                break;
            std::this_thread::sleep_for(100ms);
        }

        cancel_reading = false;
        isStopped = true;
    }
    return pid;
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
    return _pid;
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
    while (1)
    {
        std::this_thread::sleep_for(10ms);
        appFork();

        if (!need_to_another_attempt())
            break;
    }
    _watcher_guard = false;
    _pid = 0;
}

void App::run()
{
    if (!_watcher_guard)
    {
        _watcher_guard = true;
        if (_watcher_thread.joinable())
            _watcher_thread.join();
        _watcher_thread = std::thread(&App::watchFunc, this);
    }
    else
        nos::fprintln(
            "Can't start app '{}' because it's wather is already running",
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