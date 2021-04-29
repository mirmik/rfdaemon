#pragma once

#include <string>
#include <stdint.h>

#define DEFAULT_TCP_PORT		27015
#define RFMEASK_TCP_PORT		5025

void exitHandler(int sig);
bool checkRunArgs(int argc, char* argv[], uint16_t& port, std::string& appListFileName, bool& terminalMode);
int tcpServerSendThread();
int tcpServerReceiveThread();
int userIOThread();
