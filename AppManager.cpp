#include "AppManager.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include "jsoncpp/json/json.h"

using namespace std;

AppManager::AppManager()
{
}

bool AppManager::openConfigFile(const string& filename)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    bool success = false;
    ifstream appFile(filename);

    if (!appFile.is_open())
    {
        cout << "RFDaemon configuration file \"" + filename + "\" missing.\n";
        success = true;
    }
    else
        cout << "RFDaemon configuration file \"" + filename + "\" found.\n";

    if (!parseFromStream(builder, appFile, &root, &errs))
    {
        cout << "Errors in configuration file \"" + filename + "\"[" << errs << "].\n";
        success = true;
    }
    else
    {
        int arraySize = root["apps"].size();
        if (!arraySize)
        {
            cout << "No apps found in file \"" + filename + "\".\n";
            success = true;
        }
        for (int i = 0; i < arraySize; i++)
        {
            appList.push_back(root["apps"][i]["name"].asString());
            appCmdList.push_back(root["apps"][i]["command"].asString());
        }
        if (appList.empty())
        {
            cout << "No apps found in file \"" + filename + "\"\n";
            success = true;
        }
        if (appList.size() != appCmdList.size())
        {
            cout << "Property 'command' missing in file \"" + filename + "\".\n";
            success = true;
        }

        for (auto& s : appCmdList)
        {
            if (!s.empty())
            {
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
                appCmdArgList.push_back(args);
            }
            appRestartEnableList.push_back(1);
        }
        appRestartAttempts.resize(arraySize);
        appRunningStatusList.resize(arraySize);
    }
    return success;
}

void AppManager::runApps()
{
    appPidList.clear();
    size_t cmdCount = appCmdList.size();
    vector<vector<const char*>> args(cmdCount);
    vector<pid_t> pids;

    for (size_t i = 0; i < cmdCount; i++)
    {
        for (auto& item : appCmdArgList[i])
            args[i].push_back(item.c_str());
        args[i].push_back(NULL);
    }

    for (size_t i = 0; i < cmdCount; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            int status = execvp(appCmdList[i].c_str(), (char**)args[i].data());
            cout << "App \"" << appCmdList[i] << "\" exit status: " << status << endl;
            exit(status);
        }
        else if (pid > 0)
            pids.push_back(pid);
        else
            cout << "Error: failed to start process " << appCmdList[i] << "." << endl;
        cout << pid << " " << "parent: " << getpid() << endl;
    }
    appsIsRunningFlag = cmdCount > 0;
    appPidList = pids;
}

pid_t AppManager::runApp(const string& name, const string& cmd, const vector<string>& args)
{
    vector<const char*> argsStr;
    pid_t pid = 0;

    for (auto& item : args)
        argsStr.push_back(item.c_str());
    argsStr.push_back(NULL);

    appCmdArgList.push_back(args);
    appCmdList.push_back(cmd);
    appList.push_back(name);

    pid = fork();
    if (pid == 0)
    {
        int status = execvp(cmd.c_str(), (char**)argsStr.data());
        cout << "App \"" << cmd << "\" exit status: " << status << endl;
        exit(status);
    }
    else if (pid < 0)
        cout << "Error: failed to start process " << cmd << "." << endl;
    cout << pid << " " << "parent: " << getpid() << endl;
    appPidList.push_back(pid);
    return pid;
}

void AppManager::closeApps()
{
    for (auto& p : appPidList)
        kill(p, SIGTERM);
    appPidList.clear();
    cout << "All created processes have just been killed." << endl;
}

int AppManager::thread()
{
    while (1)
    {
        int sumAttempts = 0;
        for (auto& a : appRestartAttempts)
            sumAttempts += a;

        usleep(APP_CHECK_INTERVAL_US + sumAttempts * APP_CHECK_INTERVAL_US);

        for (size_t i = 0; i < appPidList.size(); i++)
        {
            if (appRestartEnableList[i] != 0)
            {
                if (kill(appPidList[i], 0) == -1)
                {
                    appRunningStatusList[i] = 0;
                    appPidList[i] = runApp(appList[i], appCmdList[i], appCmdArgList[i]);
                    if (appPidList[i] == -1)
                    {
                        if (appRestartAttempts[i] < APP_MAX_SUCCESSIVE_RESTART_ATTEMPTS)
                            appRestartAttempts[i]++;
                        else
                        {
                            appRestartEnableList[i] = 0;
                            appRestartAttempts[i] = 0;
                        }
                    }
                    else
                    {
                        appRestartAttempts[i] = 0;
                        appRunningStatusList[i] = 1;
                    }
                }
            }
        }
    }
    return 0;
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

const vector<pid_t>& AppManager::getAppPids() const
{
    return appPidList;
}

const vector<uint8_t>& AppManager::getAppStatusList() const
{
    return appRunningStatusList;
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

const std::string& AppManager::getLogFile() const
{
    return logFileStr;
}

