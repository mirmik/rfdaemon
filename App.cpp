#include "App.h"
#include <string.h>
#include "unistd.h"
#include "signal.h"
#include <sys/wait.h>
#include <iostream>
#include <filesystem>

using namespace std;

App::App(const string& name, const string& cmd, RestartMode mode)
{
    // Split string to command and arguments
    size_t argsBegin = cmd.find_first_of(' ');
    _cmd = cmd.substr(0, argsBegin);
    _name = name;
    if (mode != RestartMode::NEVER)
    if (argsBegin != string::npos)
    {
        string argstr = cmd.substr(argsBegin);
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
    isStopped = true;
    if (atStart)
        successStart = false;
    kill(_pid, SIGTERM);
    if (_thread)
    {
        if (_thread->joinable())
            _thread->join();
        delete _thread;
        _thread = NULL;
    }
}

// Call only in a separate thread
pid_t App::appFork()
{
    isStopped = false;
    isFinished = false;
    successStart = true;
    _exitStatus = 0;
    _restartAttempts++;
    pid_t pid = fork();

    if (pid == 0)
    {
        vector<char*> argsStr;
        argsStr.push_back((char*)"/bin/sh");
        argsStr.push_back((char*)"-c");
        string c = _cmd;
        for (auto& item : _args)
            c.append(" " + item);
        argsStr.push_back((char*)c.c_str());
        argsStr.push_back((char*)NULL);
        exit(execv("/bin/sh", argsStr.data()));
    }
    else if (pid > 0)
    {
        _pid = pid;
        waitFinish();
    }
    return pid;
}

bool App::stopped() const
{
    return isStopped;
}

bool App::finished() const
{
    return isFinished;
}

App::RestartMode App::restartMode() const
{
    return _restartMode;
}

bool App::successfulStart() const
{
    return successStart;
}

int App::pid() const
{
    return _pid;
}

const string& App::name() const
{
    return _name;
}

const string& App::command() const
{
    return _cmd;
}

const vector<string>& App::args() const
{
    return _args;
}

int App::exitStatus() const
{
    return _exitStatus;
}

void App::setPid(int pid)
{
    _pid = pid;
}

int64_t App::uptime() const
{
    string file = "/proc/" + to_string(_pid) + "/stat";
    auto time = filesystem::last_write_time(file);
    return time.time_since_epoch().count();
}

uint32_t App::restartAttempts() const
{
    return _restartAttempts;
}

pid_t App::waitFinish()
{
    waitpid(_pid, &_exitStatus, 0);
    isFinished = true;
    return _pid;
}

void App::watchFunc()
{
    while (1)
    {
        this_thread::sleep_for(10ms);
        if (!isStopped)
            appFork();
        if (_restartMode == RestartMode::NEVER)
            break;
        if (_restartMode == RestartMode::ERROR && !_exitStatus)
            break;
        if (!successStart)
            break;
    }
}

void App::run()
{
    if (!_thread)
        _thread = new thread(&App::watchFunc, this);
}
