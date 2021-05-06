#pragma once

#include <sched.h>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <list>
#include "App.h"

constexpr auto APP_MAX_RESTART_ATTEMPTS = 3;

class AppManager
{
public:
    enum Errors
    {
        NoError = 0,
        AppListNotFound,
        AppListSyntaxError,
        AppListConfigPath,
        AppListRuntimePath,
        AppAttemptsEmpty
    };
    AppManager(const std::string& appListFileName);
    bool loadConfigFile();
    void runApps();
    void closeApps();
    void restartApps();
    std::vector<App>& getAppsList();
    size_t getAppCount() const;
    const std::string& getAppConfigFilename();
    const std::string& getLogFilename();
    const std::string& getDeviceDescFilename() const; // rfmeask "config.json" file with path
    const std::string& getDeviceRuntimeFilename() const; // rfmeask "runtime.json" file with path
    std::list<uint8_t>& errors();
    void pushError(Errors error);
private:
    void stopRestartWatcher();
    int restartWatchFunc();
    bool cancelWatchRestart = false;
    bool restartWatcherActive = false;
    std::vector<App> apps;
    std::list<uint8_t> errorList;
    std::thread appRestartWatchThread;
    std::string logFilename;
    std::string appFilename;
    std::string settingsFilename;
    std::string runtimeSettingsFilename;
    static std::mutex ioMutex;
};
