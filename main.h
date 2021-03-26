#pragma once

#include <sched.h>
#include <vector>
#include <string>

#define DEFAULT_TCP_PORT		27015
#define RFMEASK_TCP_PORT		5025
#define APP_CHECK_INTERVAL_US	200000
#define APP_MAX_SUCCESSIVE_RESTART_ATTEMPTS	5

bool checkRunArgs(int argc, char* argv[], uint16_t& port, std::string& appListFileName, uint8_t& terminalMode);
void* tcpServerSendThread(void* arg);
void* tcpServerReceiveThread(void* arg);
void* appWatcherThread(void* arg);
void* userIOThread(void* arg);
