#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include "parameter.h"

class RFDaemon
{
public:
    RFDaemon(uint16_t port, uint16_t bufferLen = 65535);
    // Non-returnable working function, needs to be run in a different thread
    int exec();
private:
    int parseInput(uint8_t* buffer, uint8_t* output, uint32_t size);
    uint32_t packParamData(uint8_t* dst, uint8_t dev, uint32_t dstSize);
    uint32_t packParamData(uint8_t* dst, uint8_t dev, uint32_t dstSize, uint16_t paramId);
    typedef enum
    {
        GET_SERVICES_INFO = 1 << 0,
        SERVICES_START = 1 << 1,
        SERVICES_STOP = 1 << 2,
        SERVICES_RESTART = 3 << 1,
        GET_DEVICES = 1 << 3,
        GET_LOGS = 1 << 4,
        SET_STREAMING = 1 << 5,
        AXES_LIMITS_SET = 1 << 6,
        AXES_LIMITS_GET = 1 << 7,
        ZERO_ALL_AXES = 1 << 8,
        HOME_ALL_AXES = 1 << 9,
        STARTUP_SCRIPT_GET = 1 << 10,
        STARTUP_SCRIPT_SET = 1 << 11,
        UPDATE_IMG = 1 << 12,
        UPDATE_FIRMWARE = 1 << 13,
        PARAMS_READ = 1 << 14,
        PARAMS_WRITE = 1 << 15,
        AXIS_SETZERO = 1 << 16,
        AXIS_HOME = 1 << 17,
        AXIS_SETPOS = 1 << 18,
        AXIS_STOP = 1 << 19,
        AXIS_JOG = 1 << 20,
        STREAMING_DATA = 1 << 21,
        SET_RUNNABLE_APPS_LIST = 1 << 22,
        GET_RUNNABLE_APPS_LIST = 1 << 23,
        BEGIN_MULTIPACKET_READ = 1 << 24,
        MULTIPACKET_DATA_READ = 1 << 25,
        END_MULTIPACKET_READ = 1 << 26,
        BEGIN_MULTIPACKET_WRITE = 1 << 27,
        MULTIPACKET_DATA_WRITE = 1 << 28,
        END_MULTIPACKET_WRITE = 1 << 29,
        ACK = 1 << 30,
        IS_ANSWER = 1 << 31
    } Cmd;
    typedef struct
    {
        double min;
        double max;
    } Limits;
    sockaddr_in sAddr;
    int socketDesc = 0;
    int connDesc = 0;
    uint8_t streamingDevNum = 0;
    bool streamingEnabled = 0;
    uint8_t* fileReceivePtr = NULL;
    uint8_t* fileTransmitPtr = NULL;
    uint32_t fileReceiveSize = 0;
    uint32_t fileTransmitSize = 0;
    std::vector<uint8_t> rxBuf;
    std::vector<uint8_t> txBuf;
    std::vector<Limits> limitsList;
    std::vector<uint8_t> bigFileData;
    uint16_t bufferLength = 0;
    bool terminate = false;
};
