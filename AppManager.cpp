#include <iostream>
#include <fstream>
#include <mutex>
#include <unistd.h>
#include <pwd.h>
#include "jsoncpp/json/json.h"
#include "AppManager.h"

using namespace std;
std::mutex AppManager::ioMutex;

AppManager::AppManager(const string& appListFileName)
{
    appFilename = appListFileName;
}

bool AppManager::loadConfigFile()
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    ifstream appFile = ifstream(appFilename);
    bool error = false;
    string homedir = string(getpwuid(getuid())->pw_dir);
    if (appFile.is_open())
        cout << "RFDaemon configuration file \"" + appFilename + "\" found.\n";
    else
    {
        cout << "RFDaemon configuration file \"" + appFilename + "\" missing.\n";
        pushError(Errors::AppListNotFound);
        return true;
    }

    if (!parseFromStream(builder, appFile, &root, &errs))
    {
        cout << "Errors in configuration file \"" + appFilename + "\"[" << errs << "].\n";
        pushError(Errors::AppListSyntaxError);
        error = true;
    }
    else
    {
        int arraySize = root["apps"].size();
        if (!arraySize)
        {
            cout << "No apps found in file \"" + appFilename + "\".\n";
            error = true;
        }
        else
        {
            closeApps();
            apps.clear();
            for (int i = 0; i < arraySize; i++)
            {
                string name = root["apps"][i]["name"].asString();
                string cmd = root["apps"][i]["command"].asString();
                if (!cmd.empty() && !name.empty())
                {
                    // Replace username in command string to actual
                    size_t homePathPos = cmd.find("/home/"), homePathEnd = cmd.find('/', homePathPos + 7);
                    while ((homePathPos != string::npos) && (homePathEnd != string::npos) && (homePathEnd > homePathPos))
                    {
                        cmd.replace(homePathPos, homePathEnd - homePathPos, homedir);
                        homePathPos = cmd.find("/home/", homePathEnd);
                        homePathEnd = cmd.find('/', homePathPos + 7);
                    }
                    App::RestartMode restartMode;
                    if (root["apps"][i]["restart"].asString() == "error")
                        restartMode = App::RestartMode::ERROR;
                    else if (root["apps"][i]["restart"].asString() == "never")
                        restartMode = App::RestartMode::NEVER;
                    else
                        restartMode = App::RestartMode::ALWAYS; // "always" and other values
                    apps.push_back({ name, cmd, restartMode });
                }
                else
                {
                    error = true;
                    break;
                }
            }
        }
    }
    if (!error)
    {
        bool configFound = false;
        bool runtimeFound = false;
        bool rfmeasFound = false;

        // Search for config.json and runtime.json filepaths in rfmeas cmd string
        size_t i = 0, j = 0, k = 0;
        for (; i < apps.size(); i++)
        {
            if (apps[i].command().find("rfmeas") != string::npos)
            {
                rfmeasFound = true;
                break;
            }
        }
        if (rfmeasFound)
        {
            for (; j < apps[i].args().size(); j++)
            {
                if (apps[i].args()[j].find("--config") != string::npos)
                {
                    configFound = true;
                    break;
                }
            }
            for (; k < apps[i].args().size(); k++)
            {
                if (apps[i].args()[j].find("--runtime") != string::npos)
                {
                    runtimeFound = true;
                    break;
                }
            }
        }
        if (configFound && runtimeFound &&
            !apps[i].args()[j + 1].empty() && !apps[i].args()[k + 1].empty())
        {
            settingsFilename = apps[i].args()[j + 1];
            runtimeSettingsFilename = apps[i].args()[k + 1];
        }
        else
        {
            settingsFilename = homedir + "/settings.json";
            runtimeSettingsFilename = homedir + "/runtime.json";
            if (!configFound)
                pushError(Errors::AppListConfigPath);
            if (!runtimeFound)
                pushError(Errors::AppListRuntimePath);
        }
    }
    return error;
}

void AppManager::runApps()
{
    stopRestartWatcher();
    cancelWatchRestart = false;
    appRestartWatchThread = thread(&AppManager::restartWatchFunc, this);
    for (auto& a : apps)
    {
        if (a.stopped())
        {
            a.clearRestartAttempts();
            a.run();
        }
    }
}

void AppManager::closeApps()
{
    stopRestartWatcher();
    for (auto& a : apps)
        a.stop();
    ioMutex.lock();
    cout << "All created processes have just been killed." << endl;
    ioMutex.unlock();
}

void AppManager::stopRestartWatcher()
{
    if (appRestartWatchThread.joinable())
    {
        cancelWatchRestart = true;
        appRestartWatchThread.join();
    }
}

int AppManager::restartWatchFunc()
{
    for (int i = 0; i < 500; i++)
    {
        this_thread::sleep_for(10ms);
        for (size_t j = 0; j < apps.size(); j++)
        {
            if (!apps[j].stopped())
            {
                if (apps[j].restartAttempts() >= APP_MAX_RESTART_ATTEMPTS)
                {
                    apps[j].stop(true);
                    pushError(Errors::AppAttemptsEmpty);
                    ioMutex.lock();
                    cout << "Process \"" << apps[j].name() << "\" did too many restarts and won't restart anymore." << endl;
                    ioMutex.unlock();
                }
            }
        }
        if (cancelWatchRestart)
        {
            cancelWatchRestart = false;
            break;
        }
    }
    ioMutex.lock();
    cout << "Restart checker closed." << endl;
    ioMutex.unlock();
    return 0;
}

void AppManager::restartApps()
{
    closeApps();
    this_thread::sleep_for(500ms);
    runApps();
}

size_t AppManager::getAppCount() const
{
    return apps.size();
}

const string& AppManager::getAppConfigFilename()
{
    return appFilename;
}

vector<App>& AppManager::getAppsList()
{
    return apps;
}

const string& AppManager::getLogFilename()
{
    return logFilename;
}

const string& AppManager::getDeviceDescFilename() const
{
    return settingsFilename;
}

const std::string& AppManager::getDeviceRuntimeFilename() const
{
    return runtimeSettingsFilename;
}

std::list<uint8_t>& AppManager::errors()
{
    return errorList;
}

void AppManager::pushError(Errors error)
{
    errorList.push_back((uint8_t)error);
    if (errorList.size() > 100)
        errorList.pop_front();
}
