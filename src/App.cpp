#include "App.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include <nos/fprint.h>
#include <igris/util/string.h>

//std::mutex App::ioMutex;
using namespace std::chrono_literals;

std::string App::status_string() const
{
    if (isStopped)
        return "stopped";
    else
        return "running";
}

std::string GetStdoutFromCommand(std::string cmd) {

    std::string data;
    FILE* stream;
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

App::App(const std::string& name, const std::string& cmd, RestartMode mode,
    const std::vector<std::string>& logs) : _name(name)
{
    tokens = igris::split(cmd);
    _restartMode = mode;
}

void App::stop(bool atStart)
{
    if (!isStopped) {
        _attempts = 0;
        kill(_pid, SIGKILL);
    }
}

void App::start()
{
    if (isStopped)
    {
        restart_attempt_counter();
        run();
    }
}

std::vector<char*> App::tokens_for_execve(const std::vector<std::string>& args)
{
    std::vector<char*> res;
    for (auto& arg : args)
    {
        res.push_back(const_cast<char*>(arg.c_str()));
    }
    res.push_back(nullptr);
    return res;
}

// Call only in a separate thread
pid_t App::appFork()
{
    nos::println("app fork");
    _exitStatus = 0;
    increment_attempt_counter();
    pid_t pid = fork();

    _startTime = std::chrono::system_clock::now();
    if (pid == 0)
    {
        nos::fprintln("Execv app : {}", tokens);
        exit(execv(tokens[0].data(), tokens_for_execve(tokens).data()));
    }
    else if (pid > 0)
    {
        isStopped = false;
        usleep(1000);
        _pid = pid;
        waitFinish();
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

const std::string& App::name() const
{
    return _name;
}

//retrurn log pathes
std::vector<std::string> App::logPaths() const
{
    return {};
}

int64_t App::uptime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now() - _startTime).count();
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
    return _attempts != 0;
}

pid_t App::waitFinish()
{
    nos::fprintln("wait finish '{}' : {}", name(), _pid);
    waitpid(_pid, &_exitStatus, 0);
    if (_exitStatus)
        _errors.push(_exitStatus);
    if (_errors.size() > 100)
        _errors.pop();
    return _pid;
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
        nos::fprintln("Can't start app '{}' because it's wather is already running", name());
}

std::queue<int>& App::errors()
{
    return _errors;
}
