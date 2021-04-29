#include <iostream>
#include <fstream>
#include <mutex>
#include <unistd.h>
#include <pwd.h>
#include "jsoncpp/json/json.h"
#include "AppManager.h"

using namespace std;
std::mutex AppManager::ioMutex;

bool AppManager::openConfigFile(const string& filename)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    appFile = ifstream(filename);
    appFileName = filename;
    bool error = !appFile.is_open();
    string homedir = string(getpwuid(getuid())->pw_dir);
    if (error)
        cout << "RFDaemon configuration file \"" + filename + "\" missing.\n";
    else
        cout << "RFDaemon configuration file \"" + filename + "\" found.\n";

    if (!parseFromStream(builder, appFile, &root, &errs))
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
        bool runtimeSettingsFound = false;
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
                    runtimeSettingsFound = true;
                    break;
                }
            }
        }
        if (configFound && runtimeSettingsFound &&
            !apps[i].args()[j + 1].empty() && !apps[i].args()[k + 1].empty())
        {
            devDescFileName = apps[i].args()[j + 1];
            runtimeSettingsFileName = apps[i].args()[k + 1];
        }
        else
        {
            devDescFileName = homedir + "/settings.json";
            runtimeSettingsFileName = homedir + "/runtime.json";
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
            a.run();
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

void AppManager::updateConfigFile(const string& newContent)
{
}

ifstream& AppManager::getAppConfigFile()
{
    return appFile;
}

const vector<App>& AppManager::getAppsList() const
{
    return apps;
}

ifstream& AppManager::getLogFile()
{
    return logFile;
}

const string& AppManager::getDeviceDescFilename() const
{
    return devDescFileName;
}

std::list<uint8_t>& AppManager::errors()
{
    return errorList;
}
