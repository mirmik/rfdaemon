#pragma once
#include "TcpServer.h"
#include <vector>
#include <string>

class AppManager;
class DeviceManager;

class RFDaemonServer : public TcpServer<RFDaemonServer>
{
public:
    typedef enum
    {
        GET_SERVICES_INFO = 1 << 0,
        SERVICES_START = 1 << 1,
        SERVICES_STOP = 1 << 2,
        SERVICES_RESTART = 3 << 1,
        GET_DEVICES = 1 << 3,
        GET_DEV_LOGS = 1 << 4,
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
        GET_APP_LOGS = 1 << 30,
        IS_ANSWER = 1 << 31
    } CmdId;
    RFDaemonServer(uint16_t port);
    CmdBufResult getAppInfo(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult startAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult stopAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult restartAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getAllDevices(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getDevErrLogs(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setStreaming(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setAxesLimits(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getAxesLimits(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult zeroAllAxes(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult homeAllAxes(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getStartupScript(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setStartupScript(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult updateSysImg(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult updateControllerFW(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getParams(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setParams(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setAxisPosToZero(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult homeAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setAxisPos(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult stopAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult jogAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult setAppsList(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getAppsList(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult beginMultiPacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult multipacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult endMultipacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult beginMultiPacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult multipacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult endMultipacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    CmdBufResult getAppLogs(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen);
    void setAppManager(AppManager* manager);
    void setDeviceManager(DeviceManager* manager);
private:
    AppManager* appMgr = NULL;
    DeviceManager* devMgr = NULL;
};
