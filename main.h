#pragma once

#include <sched.h>
#include <vector>
#include <string>
#include <stdint.h>

#define DEFAULT_TCP_PORT		27015
#define RFMEASK_TCP_PORT		5025

bool checkRunArgs(int argc, char* argv[], uint16_t& port, std::string& appListFileName, uint8_t& terminalMode);
void* tcpServerSendThread(void* arg);
void* tcpServerReceiveThread(void* arg);
void* tcpClientSendThread(void* arg);
void* tcpClientReceiveThread(void* arg);
void* appWatcherThread(void* arg);
void* userIOThread(void* arg);
