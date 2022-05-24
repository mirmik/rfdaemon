#include "App.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include <nos/fprint.h>

using namespace std;

mutex App::ioMutex;

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

App::App(const string& name, const string& cmd, RestartMode mode,
    const vector<string>& logs) : _name(name), _logPaths(logs)
{
    // Split string to command and arguments
    size_t argsBegin = cmd.find_first_of(' ');
    size_t logPathBegin = cmd.find_first_of('>');
    _cmd = cmd.substr(0, argsBegin);

    if (logPathBegin != string::npos && cmd.size() > (logPathBegin + 3))
    {
        do logPathBegin++;
        while (cmd[logPathBegin] == ' ' && cmd[logPathBegin] != '\0');
        size_t logPathEnd = cmd.find_first_of(' ', logPathBegin);
        if (logPathEnd == string::npos)
            logPathEnd = cmd.length();
        _logPaths.push_back(string(&cmd[logPathBegin], logPathEnd - logPathBegin));
        lock_guard<std::mutex> lock(ioMutex);
        printf("Console log found: %s\n", _logPaths.back().data());
    }
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
    {
        if (kill(_pid, SIGKILL) == 0)
        {
            lock_guard<std::mutex> lock(ioMutex);
            printf("Killed app '%s' with pid %d\n", name().c_str(), _pid);
        }
        else
        {
            lock_guard<std::mutex> lock(ioMutex);
            printf("Failed to kill app %s with pid %d, ", name().c_str(), _pid);
            if (errno == ESRCH)
                printf("no such process.\n");
            else
                printf("unknown error.\n");
        }
    }
    if (_shPid)
    {
        if (kill(_shPid, SIGKILL) == 0)
        {
            lock_guard<std::mutex> lock(ioMutex);
            printf("Killed sh with pid %d\n", _shPid);
        }
        else
        {
            lock_guard<std::mutex> lock(ioMutex);
            printf("Failed to kill sh with pid %d, ", _shPid);
            if (errno == ESRCH)
                printf("no such process.\n");
            else
                printf("unknown error.\n");
        }
    }
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
        nos::fprintln("Start cmd: {} args: {}", _cmd, _args);
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
                _pid = atoi(out.c_str() + pidNameStart + 1);
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

void App::clearRestartAttempts()
{
    _restartAttempts = 0;
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

const vector<string>& App::logPaths() const
{
    return _logPaths;
}

queue<int>& App::errors()
{
    return _errors;
}
