#pragma once

#include <sched.h>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include "App.h"

#define APP_CHECK_INTERVAL_US       200000
#define APP_MAX_RESTART_ATTEMPTS    5

class AppManager
{
public:
    AppManager() {};
    bool openConfigFile(const std::string& filename);
    void runApps();
    void closeApps();
    void restartApps();
    void updateConfigFile(const std::string& newContent);
    std::ifstream& getAppConfigFile();
    const std::vector<App>& getAppsList() const;
    size_t getAppCount() const;
    std::ifstream& getLogFile();
    const std::string& getDeviceDescFilename() const; // rfmeask "config.json" file with path
private:
    int restartWatchFunc();
    std::vector<App> apps;
    std::thread appRestartWatchThread;
    std::string logFileStr;
    std::string appFileName;
    std::string devDescFileName;
    std::string runtimeSettingsFileName;
    std::ifstream logFile;
    std::ifstream appFile;
    std::ifstream devDescFile;
    std::ifstream runtimeSettingsFile;
    static std::mutex ioMutex;
};
