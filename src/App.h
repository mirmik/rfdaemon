#pragma once

#include <mutex>
#include <nos/trent/trent.h>
#include <optional>
#include <pwd.h>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

class LinkedFile
{
public:
    std::string path = {};
    std::string name = {};
    bool editable = {};

    nos::trent to_trent() const;
    LinkedFile() = default;
    LinkedFile(const LinkedFile &) = default;
};

class App
{
public:
    enum RestartMode
    {
        ALWAYS = 0,
        ONCE = 1,
    };

private:
    std::string _username = {};
    uid_t _uid = 0;
    std::vector<std::string> _args = {};
    std::vector<LinkedFile> _linked_files = {};
    int task_index = {};
    std::vector<std::string> tokens = {};
    RestartMode _restartMode = RestartMode::ALWAYS;
    std::string _name = {};
    std::queue<int> _errors = {};
    std::unordered_map<std::string, std::string> _env;

    // systemd integration
    std::string _service_name = {};   // e.g. "rfd-myapp"
    std::string _service_path = {};   // e.g. "/etc/systemd/system/rfd-myapp.service"

public:
    bool isStopped = true;

public:
    App(int task_index, const std::string &name, const std::string &cmd,
        RestartMode mode, const std::vector<LinkedFile> &linkeds,
        std::string user);

    App(const App &) = delete;
    App &operator=(const App &) = delete;
    App(App &&) = default;
    App &operator=(App &&) = default;

    // Lifecycle management (via systemd)
    void stop();
    void start();
    void restart();
    bool stopped() const;

    // Getters
    void set_user(const std::string &user);
    RestartMode restartMode() const;
    int pid() const;
    const std::string &name() const;
    const std::vector<std::string> &args() const;
    int64_t uptime() const;
    std::vector<std::string> logPaths() const;
    std::queue<int> &errors();
    std::string status_string() const;
    std::string command() const;

    // Journal/logs
    std::string get_journal_data(int lcount) const;
    std::string show_stdout() const;

    void set_environment_variables(
        const std::unordered_map<std::string, std::string> &env);

    std::string token_list_as_string() const;

    bool is_runned() const
    {
        return !isStopped;
    }

    const std::vector<LinkedFile> &linked_files() const
    {
        return _linked_files;
    }

    // Runtime editing methods
    void setCommand(const std::string &cmd);
    void setRestartMode(RestartMode mode);
    void setName(const std::string &name);
    nos::trent toTrent() const;

    // systemd service file management
    std::string generate_service_content() const;
    bool sync_service_file();
    const std::string &service_name() const { return _service_name; }
    const std::string &service_path() const { return _service_path; }
    static bool daemon_reload();

    ~App() = default;

private:
    std::string exec_command(const std::string &cmd) const;
    int exec_command_status(const std::string &cmd) const;
};
