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
    bool openConfigFile(const std::string& filename);
    void runApps();
    pid_t runApp(const std::string& name, const std::string& cmd, const std::vector<std::string>& args);
    void closeApps();
    void restartApps();
    int thread();
    void updateConfigFile(const std::string& newContent);
    std::ifstream& getAppConfigFile();
    const std::vector<pid_t>& getAppPids() const;
    const std::vector<uint8_t>& getAppStatusList() const;
    const std::vector<std::string>& getAppNames() const;
    const std::vector<std::string>& getAppCmds() const;
    const std::vector<std::vector<std::string>>& getAppArgs() const;
    uint32_t getAppCount() const;
    bool appsIsRunning() const;
    std::ifstream& getLogFile();
    const std::string& getDeviceDescFilename() const; // rfmeask "config.json" file with path
private:
    std::vector<int> appRestartEnableList;
    std::vector<uint8_t> appRunningStatusList;
    std::vector<int> appRestartAttempts;
    std::vector<pid_t> appPidList;
    std::vector<std::string> appList, appCmdList;
    std::vector<std::vector<std::string>> appCmdArgList;
    bool appsIsRunningFlag = false;
    std::string logFileStr;
    std::string cfgFileName;
    std::string devDescFileName;
    std::ifstream appConfFile;
    std::ifstream devDescFile;
    std::ifstream appLogFile;
};
