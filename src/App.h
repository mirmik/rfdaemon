#pragma once

#include <igris/trent/trent.h>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

class LinkedFile
{
public:
    std::string path = {};
    std::string name = {};
    bool editable = {};

    igris::trent to_trent() const;
    LinkedFile() = default;
    LinkedFile(const LinkedFile &) = default;
};

class App
{
public:
    enum RestartMode
    {
        ALWAYS = 0,
        ERROR,
        NEVER
    };
    App(int task_index, const std::string &name, const std::string &cmd,
        RestartMode mode, const std::vector<LinkedFile> &linkeds);
    void stop();
    void start();
    bool stopped() const;
    RestartMode restartMode() const;
    int pid() const;
    const std::string &name() const;
    const std::vector<std::string> &args() const;
    int64_t uptime() const;
    uint32_t restartAttempts() const;
    pid_t waitFinish();
    void run();
    std::vector<std::string> logPaths() const;
    std::queue<int> &errors();
    std::string status_string() const;

    bool need_to_another_attempt() const;
    void increment_attempt_counter();
    void restart_attempt_counter();

    static std::vector<char *>
    tokens_for_execve(const std::vector<std::string> &args);

    void logdata_append(const char *data, size_t size);
    size_t logdata_size() const;
    void logdata_clear();
    int64_t logdata_read(char *data, size_t size, size_t offset);

    const std::string &show_stdout() const;
    const std::vector<LinkedFile> &linked_files() const
    {
        return _linked_files;
    }

private:
    void watchFunc();
    pid_t appFork();

private:
    std::vector<std::string> _args;
    std::vector<LinkedFile> _linked_files;
    int task_index;
    std::chrono::time_point<std::chrono::system_clock> _startTime;
    std::vector<std::string> tokens;
    int32_t _attempts_initializer = 5;
    int32_t _attempts = _attempts_initializer;
    bool _watcher_guard = false;
    std::thread _watcher_thread = {};
    bool isStopped = true;
    int _exitStatus = 0;
    RestartMode _restartMode = RestartMode::ALWAYS;
    std::string _name;
    std::queue<int> _errors;
    int _pid = 0;

    std::string _stdout;
};
