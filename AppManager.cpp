#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include "jsoncpp/json/json.h"
#include "AppManager.h"
#include <sys/stat.h>
#include <sys/wait.h>

using namespace std;

vector<int> AppManager::watchingThreadsRet;
vector<int> AppManager::watchingThreadsArgs;
vector<pthread_t> AppManager::watchingThreads;
vector<int> AppManager::appRestartEnableList;
vector<uint8_t> AppManager::appRunningStatusList;
vector<int> AppManager::appRestartAttempts;
vector<pid_t> AppManager::appPidList;
vector<string> AppManager::appList, AppManager::appCmdList;
vector<vector<string>> AppManager::appCmdArgList;
bool AppManager::appsIsRunningFlag = false;
string AppManager::logFileStr;
string AppManager::cfgFileName;
string AppManager::devDescFileName;
ifstream AppManager::appConfFile;
ifstream AppManager::devDescFile;
ifstream AppManager::appLogFile;
pthread_mutex_t AppManager::stdOutputMutex;
pthread_mutex_t AppManager::threadArrayMutex;
pid_t AppManager::lastLaunchPidResult = 0;

AppManager::AppManager()
{
    devDescFileName = "";
    pthread_mutex_init(&stdOutputMutex, NULL);
    pthread_mutex_init(&threadArrayMutex, NULL);
}

bool AppManager::openConfigFile(const string& filename)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    bool error = false;
    appConfFile = ifstream(filename);
    string homedir = string(getpwuid(getuid())->pw_dir);

    if (!appConfFile.is_open())
    {
        cout << "RFDaemon configuration file \"" + filename + "\" missing.\n";
        error = true;
    }
    else
        cout << "RFDaemon configuration file \"" + filename + "\" found.\n";

    if (!parseFromStream(builder, appConfFile, &root, &errs))
    {
        cout << "Errors in configuration file \"" + filename + "\"[" << errs << "].\n";
        error = true;
    }
    else
    {
        int arraySize = root["apps"].size();
        if (!arraySize)
        {
            cout << "No apps found in file \"" + filename + "\".\n";
            error = true;
        }
        else
        {
            for (int i = 0; i < arraySize; i++)
            {
                appList.push_back(root["apps"][i]["name"].asString());
                appCmdList.push_back(root["apps"][i]["command"].asString());
            }
            if (appList.size() != appCmdList.size())
            {
                cout << "Property 'command' missing in file \"" + filename + "\".\n";
                error = true;
            }

            for (auto& s : appCmdList)
            {
                if (!s.empty())
                {
                    // Replace username in command string to actual
                    size_t homePathPos = s.find("/home/");
                    size_t homePathEnd = s.find('/', homePathPos + 7);
                    while ((homePathPos != string::npos) && (homePathEnd != string::npos) && (homePathEnd > homePathPos))
                    {
                        s.replace(homePathPos, homePathEnd - homePathPos, homedir);
                        homePathPos = s.find("/home/", homePathEnd);
                        homePathEnd = s.find('/', homePathPos + 7);
                    }

                    // Split string to command and arguments
                    size_t argsBegin = s.find_first_of(' ');
                    vector<string> args = { s.substr(0, argsBegin) };

                    if (argsBegin != string::npos)
                    {
                        string argstr = s.substr(argsBegin);

                        char* p = strtok((char*)argstr.c_str(), " ");
                        while (p)
                        {
                            args.push_back(p);
                            p = strtok(NULL, " ");
                        }
                    }
                    s = s.substr(0, argsBegin);
                    appCmdArgList.push_back(args);
                }
                appRestartEnableList.push_back(1);
            }
            appRestartAttempts.resize(arraySize);
            appRunningStatusList.resize(arraySize);
        }
    }
    if (!error)
    {
        bool configFound = false;
        cfgFileName = filename;

        size_t i = 0, j = 0;
        for (; i < appCmdList.size(); i++)
        {
            if (appCmdList[i].find("rfmeas") != string::npos)
                break;
        }
        for (;j < appCmdArgList[i].size(); j++)
        {
            if (appCmdArgList[i][j].find("--config") != string::npos)
            {
                configFound = true;
                break;
            }
        }

        if (configFound && !appCmdArgList[i][j + 1].empty())
            devDescFileName = appCmdArgList[i][j + 1];
        else
            devDescFileName = homedir + "/settings.json";
    }
    return error;
}

void AppManager::runApps()
{
    appPidList.clear();
    size_t cmdCount = appCmdList.size();
    vector<vector<const char*>> args(cmdCount);
    vector<pid_t> pids;

    pthread_mutex_lock(&threadArrayMutex);
    for (size_t i = 0; i < cmdCount; i++)
    {
        for (auto& item : appCmdArgList[i])
            args[i].push_back(item.c_str());
        args[i].push_back(NULL);
        watchingThreads.push_back(0);
        watchingThreadsRet.push_back(0);
        watchingThreadsArgs.push_back(i);
    }
    pthread_mutex_unlock(&threadArrayMutex);
    for (size_t i = 0; i < cmdCount; i++)
        pthread_create(&watchingThreads[i], NULL, appWatchThread, &watchingThreadsArgs[i]);
    appsIsRunningFlag = cmdCount > 0;
    appPidList = pids;
}

pid_t AppManager::runNewApp(const string& name, const string& cmd, const vector<string>& args)
{
    int result = 0;
    vector<const char*> argsStr;
    for (auto& item : args)
        argsStr.push_back(item.c_str());
    argsStr.push_back(NULL);

    pthread_mutex_lock(&threadArrayMutex);
    appCmdArgList.push_back(args);
    appCmdList.push_back(cmd);
    appList.push_back(name);
    watchingThreads.push_back(0);
    watchingThreadsRet.push_back(0);
    watchingThreadsArgs.push_back(appList.size() - 1);
    pthread_mutex_unlock(&threadArrayMutex);
    pthread_create(&watchingThreads.back(), NULL, appWatchThread, &watchingThreadsArgs.back());
    if (lastLaunchPidResult > 0)
        result = appPidList.back();
    else
        result = lastLaunchPidResult;
    return result;
}

void AppManager::closeApps()
{
    for (size_t i = 0; i < appPidList.size(); i++)
    {
        pthread_mutex_lock(&threadArrayMutex);
        appRestartEnableList[i] = 0;
        pthread_mutex_unlock(&threadArrayMutex);
        kill(appPidList[i], SIGTERM);
    }
    pthread_mutex_lock(&threadArrayMutex);
    appPidList.clear();
    pthread_mutex_unlock(&threadArrayMutex);
    pthread_mutex_lock(&stdOutputMutex);
    cout << "All created processes have just been killed." << endl;
    cout.flush();
    pthread_mutex_unlock(&stdOutputMutex);
}

AppManager::~AppManager()
{
    pthread_mutex_destroy(&stdOutputMutex);
    pthread_mutex_destroy(&threadArrayMutex);
}

const string& AppManager::getDeviceDescFilename() const
{
    return devDescFileName;
}

void* AppManager::appWatchThread(void* arg)
{
    const int& threadNum = *(int*)arg;
    vector<char*> args;
    for (auto& item : appCmdArgList[threadNum])
        args.push_back((char*)item.c_str());

    while (1)
    {
        if (!appRunningStatusList[threadNum])
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                int status = execv(appCmdList[threadNum].c_str(), args.data());
                pthread_mutex_lock(&stdOutputMutex);
                cout << "Process \"" << appCmdList[threadNum] << "\" exit status: " << status << endl;
                cout.flush();
                pthread_mutex_unlock(&stdOutputMutex);
            }
            else if (pid > 0)
            {
                pthread_mutex_lock(&threadArrayMutex);
                lastLaunchPidResult = pid;
                appPidList.push_back(pid);
                pthread_mutex_unlock(&threadArrayMutex);
                appRunningStatusList[threadNum] = 1;
                wait(&watchingThreadsRet[threadNum]);
            }
            else
            {
                lastLaunchPidResult = pid;
                pthread_mutex_lock(&stdOutputMutex);
                cout << "Error: failed to start process \"" << appCmdList[threadNum] << "\"." << endl;
                cout.flush();
                pthread_mutex_unlock(&stdOutputMutex);
            }
            pthread_mutex_lock(&stdOutputMutex);
            cout << pid << " " << "parent: " << getpid() << endl;
            cout.flush();
            pthread_mutex_unlock(&stdOutputMutex);
        }
        appRunningStatusList[threadNum] = 0;
        if (!appRestartEnableList[threadNum])
            break;
        usleep(1000);
    }
    return &watchingThreadsRet[threadNum];
}

void AppManager::restartApps()
{
    closeApps();
    usleep(APP_CHECK_INTERVAL_US);
    runApps();
}

uint32_t AppManager::getAppCount() const
{
    return appPidList.size();
}

bool AppManager::appsIsRunning() const
{
    return appsIsRunningFlag;
}

void AppManager::updateConfigFile(const string& newContent)
{
}

ifstream& AppManager::getAppConfigFile()
{
    return appConfFile;
}

const vector<pid_t>& AppManager::getAppPids() const
{
    return appPidList;
}

const vector<uint8_t>& AppManager::getAppStatusList() const
{
    return appRunningStatusList;
}

const vector<uint64_t> AppManager::getAppUptimeList() const
{
    vector<uint64_t> uptimes;

    for (size_t i = 0; i < appList.size(); i++)
    {
        string file = "/proc/" + to_string(appPidList[i]) + "/stat";
        struct stat attrib;
        stat(file.c_str(), &attrib);

        uptimes.push_back(time(NULL) - attrib.st_mtime);
    }
    return uptimes;
}

const vector<string>& AppManager::getAppNames() const
{
    return appList;
}

const vector<string>& AppManager::getAppCmds() const
{
    return appCmdList;
}

const vector<vector<string>>& AppManager::getAppArgs() const
{
    return appCmdArgList;
}

ifstream& AppManager::getLogFile()
{
    return appLogFile;
}
