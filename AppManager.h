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
        AppAttemptsEmpty,
        AppRfmeasNotFound
    };
    struct Log
    {
        std::string path;
        std::vector<uint8_t> data; // First 4 bytes are the size of data
    };
    AppManager(const std::string& appListFileName);
    bool loadConfigFile();
    void runApps();
    void closeApps();
    void restartApps();
    std::vector<App>& getAppsList();
    size_t getAppCount() const;
    const std::string& getAppConfigFilename();
    const std::string& getDeviceDescFilename() const; // rfmeask "config.json" file with path
    const std::string& getDeviceRuntimeFilename() const; // rfmeask "runtime.json" file with path
    std::list<uint8_t>& errors();
    std::vector<std::string>& getSystemLogPaths();
    void pushError(Errors error);
    std::vector<AppManager::Log> packLogs();
private:
    void stopRestartWatcher();
    int restartWatchFunc();
    bool cancelWatchRestart = false;
    bool restartWatcherActive = false;
    std::vector<App> apps;
    std::vector<std::string> systemLogPaths;
    std::list<uint8_t> errorList;
    std::thread appRestartWatchThread;
    std::string appFilename;
    std::string settingsFilename;
    std::string runtimeSettingsFilename;
    static std::mutex ioMutex;
};
