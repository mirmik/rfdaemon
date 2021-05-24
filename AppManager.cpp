#include <iostream>
#include <fstream>
#include <mutex>
#include <unistd.h>
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
    bool configFound = false;
    bool runtimeFound = false;
    bool rfmeasFound = false;
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    ifstream appFile = ifstream(appFilename);
    bool error = false;
    settingsFilename = "/home/rfmeas/settings.json";
    runtimeSettingsFilename = "/home/rfmeas/runtime.json";
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
                    App::RestartMode restartMode;
                    if (root["apps"][i]["restart"].asString() == "error")
                        restartMode = App::RestartMode::ERROR;
                    else if (root["apps"][i]["restart"].asString() == "never")
                        restartMode = App::RestartMode::NEVER;
                    else
                        restartMode = App::RestartMode::ALWAYS;
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
                    if (!apps[i].args()[j + 1].empty())
                    {
                        configFound = true;
                        settingsFilename = apps[i].args()[j + 1];
                        break;
                    }
                }
            }
            for (; k < apps[i].args().size(); k++)
            {
                if (apps[i].args()[k].find("--runtime") != string::npos)
                {
                    if (!apps[i].args()[k + 1].empty())
                    {
                        runtimeFound = true;
                        runtimeSettingsFilename = apps[i].args()[k + 1];
                        break;
                    }
                }
            }
            if (!configFound)
                pushError(Errors::AppListConfigPath);
            if (!runtimeFound)
                pushError(Errors::AppListRuntimePath);
        }
        else
            pushError(Errors::AppRfmeasNotFound);
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
        a.clearRestartAttempts();
        if (a.stopped())
            a.run();
    }
}

void AppManager::closeApps()
{
    stopRestartWatcher();
    for (auto& a : apps)
    {
        if (!a.stopped())
            a.stop();
    }
    lock_guard lock(ioMutex);
    cout << "All created processes have just been killed." << endl;
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
                    lock_guard lock(ioMutex);
                    cout << "Process \"" << apps[j].name() << "\" did too many restarts and won't restart anymore." << endl;
                }
            }
        }
        if (cancelWatchRestart)
        {
            cancelWatchRestart = false;
            break;
        }
    }
    lock_guard lock(ioMutex);
    cout << "Restart checker closed." << endl;
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

size_t AppManager::getLogFilesCount() const
{
    size_t logCount = 0;
    for (const auto& a : apps)
    {
        if (!a.logPath().empty())
            logCount++;
    }
    return logCount;
}

const string& AppManager::getAppConfigFilename()
{
    return appFilename;
}

vector<App>& AppManager::getAppsList()
{
    return apps;
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
