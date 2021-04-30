#include "App.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <thread>

using namespace std;

string GetStdoutFromCommand(string cmd) {

    string data;
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
    if (_pid)
        kill(_pid, SIGTERM);
    if (_shPid)
        kill(_shPid, SIGTERM);
    _restartAttempts = 0;
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
        _shPid = pid;
        usleep(1000);
        if (kill(_shPid, 0) == 0)
        {
            string out = GetStdoutFromCommand("pstree -pls " + to_string(_shPid));
            size_t procNameStart = out.find(_name);
            size_t pidNameStart = out.find('(', procNameStart);
            if (procNameStart != string::npos && pidNameStart != string::npos)
                _pid = std::atoi(out.c_str() + pidNameStart + 1);
        }
        else
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

int64_t App::uptime() const
{
    string file = "/proc/" + to_string(_shPid) + "/stat";
    struct stat buf;
    int64_t result = 0;
    if (!stat(file.c_str(), &buf))
        result = time(NULL) - buf.st_mtim.tv_sec;
    return result;
}

uint32_t App::restartAttempts() const
{
    return _restartAttempts;
}

pid_t App::waitFinish()
{
    waitpid(_shPid, &_exitStatus, 0);
    isFinished = true;
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
        this_thread::sleep_for(10ms);
        if (!isStopped)
            appFork();
        else
            return;
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
    isStopped = false;
    if (!_thread)
        _thread = new thread(&App::watchFunc, this);
}

std::queue<int8_t>& App::errors()
{
    return _errors;
}
