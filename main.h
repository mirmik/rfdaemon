#pragma once

#include <sched.h>
#include <vector>
#include <string>

#define DEFAULT_TCP_PORT		27015
#define APP_CHECK_INTERVAL_US	200000
#define APP_MAX_SUCCESSIVE_RESTART_ATTEMPTS	5

bool checkRunArgs(int argc, char* argv[], uint16_t& port, std::string& appList, uint8_t& terminalMode);
bool checkConfigFile(const std::string& filename, std::vector<std::string>& apps, std::vector<std::string>& cmds, std::vector<std::vector<std::string>>& cmdArgs);
std::vector<pid_t> runApps(const std::vector<std::string>& cmds, const std::vector<std::vector<std::string>>& cmdArgs);
pid_t runApp(const std::string& cmd, const std::vector<std::string>& args);
void closeApps(const std::vector<pid_t>& appPidList);
void* tcpServerThread(void* arg);
void* appWatcherThread(void* arg);
void* userIOThread(void* arg);
