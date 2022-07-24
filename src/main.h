#pragma once

#include <stdint.h>
#include <string>

#define DEFAULT_RFDAEMON_PROTO_PORT 27015

void exitHandler(int sig);
bool checkRunArgs(int argc, char *argv[]);
int tcpServerSendThreadHandler();
int tcpServerReceiveThreadHandler();
int userIOThreadHandler();
