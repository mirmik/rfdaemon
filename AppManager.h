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
    AppManager() {};
    bool openConfigFile(const std::string& filename);
    void runApps();
    void closeApps();
    void restartApps();
    void updateConfigFile(const std::string& newContent);
    std::ifstream& getAppConfigFile();
    std::vector<App>& getAppsList();
    size_t getAppCount() const;
    std::ifstream& getLogFile();
    const std::string& getDeviceDescFilename() const; // rfmeask "config.json" file with path
    std::list<uint8_t>& errors();
private:
    void stopRestartWatcher();
    int restartWatchFunc();
    bool cancelWatchRestart = false;
    bool restartWatcherActive = false;
    std::vector<App> apps;
    std::list<uint8_t> errorList;
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
