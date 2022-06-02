#include "App.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include <nos/fprint.h>

std::mutex App::ioMutex;
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
    const std::vector<std::string>& logs) : _name(name), _logPaths(logs)
{
    // Split std::string to command and arguments
    size_t argsBegin = cmd.find_first_of(' ');
    size_t logPathBegin = cmd.find_first_of('>');
    _cmd = cmd.substr(0, argsBegin);

    if (logPathBegin != std::string::npos && cmd.size() > (logPathBegin + 3))
    {
        do logPathBegin++;
        while (cmd[logPathBegin] == ' ' && cmd[logPathBegin] != '\0');
        size_t logPathEnd = cmd.find_first_of(' ', logPathBegin);
        if (logPathEnd == std::string::npos)
            logPathEnd = cmd.length();
        _logPaths.push_back(std::string(&cmd[logPathBegin], logPathEnd - logPathBegin));
        std::lock_guard<std::mutex> lock(ioMutex);
        printf("Console log found: %s\n", _logPaths.back().data());
    }
    if (argsBegin != std::string::npos)
    {
        std::string argstr = cmd.substr(argsBegin);
        char* p = strtok((char*)argstr.c_str(), " ");
        while (p)
        {
            _args.push_back(p);
            p = strtok(NULL, " ");
        }
    }
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

// Call only in a separate thread
pid_t App::appFork()
{
    nos::println("app fork");
    _exitStatus = 0;
    increment_attempt_counter();
    pid_t pid = fork();

    if (pid == 0)
    {
        nos::fprintln("Start cmd: {} args: {}", _cmd, _args);
        std::vector<char*> argsStr;
        std::string c = _cmd;
        for (auto& item : _args)
            argsStr.push_back((char*)c.c_str());
        argsStr.push_back((char*)NULL);
        exit(execv(argsStr[0], argsStr.data()));
    }
    else if (pid > 0)
    {
        isStopped = false;
        _shPid = pid;
        usleep(1000);
        /*if (kill(_shPid, 0) == 0)
        {
            std::string out = GetStdoutFromCommand("pstree -pls " + std::to_string(_shPid));
            size_t procNameStart = out.find(_name);
            size_t pidNameStart = out.find('(', procNameStart);
            if (procNameStart != std::string::npos && pidNameStart != std::string::npos)
                _pid = atoi(out.c_str() + pidNameStart + 1);
        }
        else*/
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

const std::string& App::command() const
{
    return _cmd;
}

const std::vector<std::string>& App::args() const
{
    return _args;
}

int App::exitStatus() const
{
    return _exitStatus;
}

int64_t App::uptime() const
{
    std::string file = "/proc/" + std::to_string(_shPid) + "/stat";
    struct stat buf;
    int64_t result = 0;
    if (!stat(file.c_str(), &buf))
        result = time(NULL) - buf.st_mtim.tv_sec;
    return result;
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
    nos::fprintln("wait finish '{}' : {} {}", name(), _shPid, _pid);
    waitpid(_shPid, &_exitStatus, 0);
    if (_exitStatus)
        _errors.push(_exitStatus);
    if (_errors.size() > 100)
        _errors.pop();
    return _shPid;
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
    _shPid = 0;
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

const std::vector<std::string>& App::logPaths() const
{
    return _logPaths;
}

std::queue<int>& App::errors()
{
    return _errors;
}
