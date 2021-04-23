#pragma once

#include <sched.h>
#include <vector>
#include <string>
#include <fstream>

#define APP_CHECK_INTERVAL_US       200000
#define APP_MAX_RESTART_ATTEMPTS    5

class AppManager
{
public:
    AppManager();
    ~AppManager();
    bool openConfigFile(const std::string& filename);
    void runApps();
    pid_t runNewApp(const std::string& name, const std::string& cmd, const std::vector<std::string>& args);
    void closeApps();
    void restartApps();
    void updateConfigFile(const std::string& newContent);
    std::ifstream& getAppConfigFile();
    const std::vector<pid_t>& getAppPids() const;
    const std::vector<uint8_t>& getAppStatusList() const;
    const std::vector<uint64_t> getAppUptimeList() const;
    const std::vector<std::string>& getAppNames() const;
    const std::vector<std::string>& getAppCmds() const;
    const std::vector<std::vector<std::string>>& getAppArgs() const;
    uint32_t getAppCount() const;
    bool appsIsRunning() const;
    std::ifstream& getLogFile();
    const std::string& getDeviceDescFilename() const; // rfmeask "config.json" file with path
private:
    static void* appWatchThread(void* arg);
    static std::vector<int> watchingThreadsRet;
    static std::vector<int> watchingThreadsArgs;
    static std::vector<pthread_t> watchingThreads;
    static std::vector<int> appRestartEnableList;
    static std::vector<uint8_t> appRunningStatusList;
    static std::vector<int> appRestartAttempts;
    static std::vector<pid_t> appPidList;
    static std::vector<std::string> appList, appCmdList;
    static std::vector<std::vector<std::string>> appCmdArgList;
    static bool appsIsRunningFlag;
    static std::string logFileStr;
    static std::string cfgFileName;
    static std::string devDescFileName;
    static std::ifstream appConfFile;
    static std::ifstream devDescFile;
    static std::ifstream appLogFile;
    static pthread_mutex_t stdOutputMutex;
    static pthread_mutex_t threadArrayMutex;
    static pid_t lastLaunchPidResult;
};
