#include "App.h"
#include "AppManager.h"
#include <array>
#include <cctype>
#include <cstdint>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nos/fprint.h>
#include <pwd.h>
#include <shutdown.h>
#include <sstream>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <igris/util/string.h>

using namespace std::chrono_literals;

extern "C"
{
struct sd_journal;

int sd_journal_open(sd_journal **ret, int flags);
void sd_journal_close(sd_journal *j);
int sd_journal_add_match(sd_journal *j, const void *data, size_t size);
int sd_journal_seek_tail(sd_journal *j);
int sd_journal_next(sd_journal *j);
int sd_journal_wait(sd_journal *j, uint64_t timeout_usec);
int sd_journal_get_data(sd_journal *j,
                        const char *field,
                        const void **data,
                        size_t *length);
int sd_journal_get_realtime_usec(sd_journal *j, uint64_t *ret_usec);
}

namespace
{
std::string journal_field_value(sd_journal *journal, const char *field)
{
    const void *data = nullptr;
    size_t length = 0;
    int ret = sd_journal_get_data(journal, field, &data, &length);
    if (ret < 0 || data == nullptr)
        return {};

    std::string entry(static_cast<const char *>(data), length);
    std::string prefix = std::string(field) + "=";
    if (entry.rfind(prefix, 0) == 0)
        return entry.substr(prefix.size());

    return entry;
}

std::string format_journal_entry(sd_journal *journal)
{
    std::ostringstream ss;

    uint64_t timestamp_usec = 0;
    if (sd_journal_get_realtime_usec(journal, &timestamp_usec) >= 0)
    {
        std::time_t timestamp_sec = static_cast<std::time_t>(timestamp_usec / 1000000);
        std::tm tm = {};
        localtime_r(&timestamp_sec, &tm);
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " ";
    }

    auto identifier = journal_field_value(journal, "SYSLOG_IDENTIFIER");
    if (identifier.empty())
        identifier = journal_field_value(journal, "_COMM");
    if (!identifier.empty())
        ss << "[" << identifier << "] ";

    auto message = journal_field_value(journal, "MESSAGE");
    if (message.empty())
        message = "<empty journal message>";
    ss << message;

    std::string line = ss.str();
    if (line.empty() || line.back() != '\n')
        line.push_back('\n');

    return line;
}
}

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
        }
    }
}

App::App(int task_index, const std::string &name, const std::string &cmd,
         RestartMode mode, const std::vector<LinkedFile> &linkeds,
         std::string user)
    : _linked_files(linkeds), task_index(task_index), _name(name)
{
    tokens = igris::split_cmdargs(cmd);
    _restartMode = mode;
    set_user(user);

    // Generate service name from app name (sanitize for systemd)
    std::string safe_name = name;
    for (char &c : safe_name)
    {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_')
            c = '_';
    }
    _service_name = "rfd-" + safe_name;
    _service_path = "/etc/systemd/system/" + _service_name + ".service";
}

std::string App::exec_command(const std::string &cmd) const
{
    std::array<char, 256> buffer;
    std::string result;

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return result;

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}

int App::exec_command_status(const std::string &cmd) const
{
    return system(cmd.c_str());
}

std::string App::generate_service_content() const
{
    std::ostringstream ss;

    ss << "# Managed by rfdaemon - DO NOT EDIT\n";
    ss << "[Unit]\n";
    ss << "Description=rfdaemon managed: " << _name << "\n";
    ss << "After=network.target\n";
    ss << "\n";

    ss << "[Service]\n";
    ss << "Type=simple\n";
    ss << "ExecStart=" << command() << "\n";

    // Map RestartMode to systemd restart policy
    if (_restartMode == RestartMode::ALWAYS)
        ss << "Restart=always\n";
    else
        ss << "Restart=no\n";

    ss << "RestartSec=5\n";

    if (!_username.empty())
        ss << "User=" << _username << "\n";

    // Environment variables
    for (const auto &kv : _env)
    {
        ss << "Environment=\"" << kv.first << "=" << kv.second << "\"\n";
    }

    ss << "WorkingDirectory=/\n";
    ss << "\n";

    ss << "[Install]\n";
    ss << "WantedBy=multi-user.target\n";

    return ss.str();
}

bool App::sync_service_file()
{
    std::string new_content = generate_service_content();

    // Check if file exists and has same content
    std::ifstream existing(_service_path);
    if (existing.is_open())
    {
        std::stringstream buffer;
        buffer << existing.rdbuf();
        std::string old_content = buffer.str();
        existing.close();

        if (old_content == new_content)
        {
            nos::fprintln("[App::sync_service_file] '{}' service file unchanged",
                          _name);
            return false; // No changes
        }
    }

    // Write new content
    std::ofstream out(_service_path);
    if (!out.is_open())
    {
        nos::fprintln(
            "[App::sync_service_file] Failed to write service file: {}",
            _service_path);
        return false;
    }

    out << new_content;
    out.close();

    nos::fprintln("[App::sync_service_file] '{}' service file updated: {}",
                  _name, _service_path);
    return true;
}

bool App::daemon_reload()
{
    nos::println("[App::daemon_reload] Running systemctl daemon-reload");
    int ret = system("systemctl daemon-reload");
    return ret == 0;
}

void App::start()
{
    nos::fprintln("[App::start] Starting '{}'...", _name);

    // Sync service file before starting
    if (sync_service_file())
    {
        daemon_reload();
    }

    std::string cmd = "systemctl start " + _service_name;
    int ret = exec_command_status(cmd.c_str());

    if (ret == 0)
    {
        isStopped = false;
        nos::fprintln("[App::start] '{}' started successfully", _name);
    }
    else
    {
        nos::fprintln("[App::start] '{}' failed to start, exit code: {}", _name,
                      ret);
    }
}

void App::stop()
{
    nos::fprintln("[App::stop] Stopping '{}'...", _name);

    std::string cmd = "systemctl stop " + _service_name;
    int ret = exec_command_status(cmd.c_str());

    isStopped = true;

    if (ret == 0)
    {
        nos::fprintln("[App::stop] '{}' stopped successfully", _name);
    }
    else
    {
        nos::fprintln("[App::stop] '{}' stop returned code: {}", _name, ret);
    }
}

void App::restart()
{
    nos::fprintln("[App::restart] Restarting '{}'...", _name);

    // Sync service file before restarting
    if (sync_service_file())
    {
        daemon_reload();
    }

    std::string cmd = "systemctl restart " + _service_name;
    int ret = exec_command_status(cmd.c_str());

    if (ret == 0)
    {
        isStopped = false;
        nos::fprintln("[App::restart] '{}' restarted successfully", _name);
    }
    else
    {
        nos::fprintln("[App::restart] '{}' restart failed, exit code: {}",
                      _name, ret);
    }
}

bool App::stopped() const
{
    std::string cmd = "systemctl is-active --quiet " + _service_name;
    int ret = system(cmd.c_str());
    return ret != 0;
}

int App::pid() const
{
    std::string cmd =
        "systemctl show -p MainPID --value " + _service_name + " 2>/dev/null";
    std::string output = exec_command(cmd);

    // Trim whitespace
    while (!output.empty() &&
           (output.back() == '\n' || output.back() == '\r' ||
            output.back() == ' '))
    {
        output.pop_back();
    }

    if (output.empty())
        return 0;

    try
    {
        return std::stoi(output);
    }
    catch (...)
    {
        return 0;
    }
}

int64_t App::uptime() const
{
    // Get ActiveEnterTimestampMonotonic from systemd
    std::string cmd = "systemctl show -p ActiveEnterTimestampMonotonic --value " +
                      _service_name + " 2>/dev/null";
    std::string output = exec_command(cmd);

    // Trim whitespace
    while (!output.empty() &&
           (output.back() == '\n' || output.back() == '\r' ||
            output.back() == ' '))
    {
        output.pop_back();
    }

    if (output.empty() || output == "0")
        return 0;

    try
    {
        // Value is in microseconds
        uint64_t start_usec = std::stoull(output);
        if (start_usec == 0)
            return 0;

        // Get current monotonic time
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        uint64_t now_usec =
            static_cast<uint64_t>(ts.tv_sec) * 1000000ULL +
            static_cast<uint64_t>(ts.tv_nsec) / 1000ULL;

        if (now_usec > start_usec)
            return static_cast<int64_t>((now_usec - start_usec) / 1000000ULL);
    }
    catch (...)
    {
    }

    return 0;
}

std::string App::status_string() const
{
    // Query systemd for actual status
    std::string cmd =
        "systemctl is-active " + _service_name + " 2>/dev/null";
    std::string output = exec_command(cmd);

    // Trim whitespace
    while (!output.empty() &&
           (output.back() == '\n' || output.back() == '\r' ||
            output.back() == ' '))
    {
        output.pop_back();
    }

    // Map systemd statuses to our standard strings for backwards compatibility
    if (output == "active" || output == "activating" || output == "reloading")
        return "running";
    if (output == "inactive" || output == "deactivating" || output == "failed" ||
        output.empty())
        return "stopped";

    return output;
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
    std::string cmd = "[";
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        cmd += tokens[i];
        if (i != tokens.size() - 1)
            cmd += ",";
    }
    cmd += "]";
    return cmd;
}

std::string App::get_journal_data(int lines_count) const
{
    std::string cmd = "journalctl -u " + _service_name +
                      " --no-pager --all -n " +
                      std::to_string(lines_count) + " 2>/dev/null";
    return exec_command(cmd);
}

std::string App::show_stdout() const
{
    // Return recent journal logs (last 100 lines)
    return get_journal_data(100);
}

bool App::stream_journal(
    const std::function<bool(const std::string &)> &sink) const
{
    sd_journal *journal = nullptr;
    int ret = sd_journal_open(&journal, 0);
    if (ret < 0 || journal == nullptr)
    {
        return sink("Failed to open systemd journal\n");
    }

    struct JournalCloser
    {
        sd_journal *journal;
        ~JournalCloser()
        {
            if (journal != nullptr)
                sd_journal_close(journal);
        }
    } closer{journal};

    const std::string match = "_SYSTEMD_UNIT=" + _service_name + ".service";
    ret = sd_journal_add_match(journal, match.c_str(), match.size());
    if (ret < 0)
    {
        return sink("Failed to filter systemd journal\n");
    }

    ret = sd_journal_seek_tail(journal);
    if (ret < 0)
    {
        return sink("Failed to seek systemd journal tail\n");
    }

    while (!is_shutdown_requested())
    {
        while ((ret = sd_journal_next(journal)) > 0)
        {
            if (!sink(format_journal_entry(journal)))
                return false;
        }

        if (ret < 0)
        {
            return sink("Failed to read from systemd journal\n");
        }

        ret = sd_journal_wait(journal, 500000);
        if (ret < 0)
        {
            return sink("Failed while waiting for journal updates\n");
        }
    }

    return true;
}

App::RestartMode App::restartMode() const
{
    return _restartMode;
}

const std::string &App::name() const
{
    return _name;
}

const std::vector<std::string> &App::args() const
{
    return tokens;
}

std::vector<std::string> App::logPaths() const
{
    // With systemd, logs are in journal
    return {};
}

std::queue<int> &App::errors()
{
    return _errors;
}

void App::set_environment_variables(
    const std::unordered_map<std::string, std::string> &env)
{
    _env = env;
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
    // Update service name when app name changes
    std::string safe_name = newName;
    for (char &c : safe_name)
    {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_')
            c = '_';
    }
    _service_name = "rfd-" + safe_name;
    _service_path = "/etc/systemd/system/" + _service_name + ".service";
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

void App::update_cached_status()
{
    // Update all cached values from systemd (called from background thread)
    isStopped = stopped();
    _cached_pid.store(pid());
    _cached_uptime.store(uptime());
}

// Update isStopped for all apps from systemd status
void AppManager::update_systemctl_projects_status()
{
    nos::fprintln("[systemd_updater] Thread started, ID: {}",
                  std::this_thread::get_id());

    while (!is_shutdown_requested())
    {
        {
            std::lock_guard<std::mutex> lock(apps_mutex);
            for (auto &app : apps)
            {
                if (is_shutdown_requested())
                    break;

                // Update all cached values from systemd
                app->update_cached_status();
            }
        }

        // Wait before next update
        if (wait_for_shutdown_ms(1000))
            break;
    }

    nos::println("[systemd_updater] Thread stopped");
}
