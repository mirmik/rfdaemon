#pragma once
#include "TcpServer.h"
#include <vector>
#include <string>
#include <fstream>

class AppManager;
class DeviceManager;
class RFDaemonServer;

typedef struct
{
    uint32_t bitCode;
    Func<RFDaemonServer, std::vector<uint8_t>, const uint8_t*, uint32_t> cmd;
} SrvCmd;

class RFDaemonServer : public TcpServer
{
public:
    typedef enum
    {
        GET_SERVICES_INFO = 0,
        SERVICES_START,
        SERVICES_STOP,
        SERVICES_RESTART,
        GET_CONFIG,
        SET_CONFIG,
        GET_DEV_LOGS,
        GET_MEASUREMENTS,
        AXES_LIMITS_SET,
        UPDATE_IMG,
        UPDATE_FIRMWARE,
        PARAMS_READ,
        PARAMS_WRITE,
        AXIS_SETZERO,
        AXIS_HOME,
        AXIS_SETPOS,
        AXIS_STOP,
        AXIS_JOG,
        SET_RUNNABLE_SERVICES_LIST,
        GET_RUNNABLE_SERVICES_LIST,
        GET_SERVICES_LOGS
    } CmdId;
    RFDaemonServer(uint16_t port);
    std::vector<uint8_t> getAppInfo(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> startAllApps(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> stopAllApps(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> restartAllApps(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getConfig(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setConfig(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getDevErrLogs(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getSystemMeasurements(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setAxesLimits(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> updateSysImg(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> updateControllerFW(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getParams(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setParams(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setAxisPosToZero(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> homeAxis(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setAxisPos(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> stopAxis(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> jogAxis(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setAppsList(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getAppsList(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getAppLogs(const uint8_t* data, uint32_t size);
    void setAppManager(AppManager* manager);
    void setDeviceManager(DeviceManager* manager);
    void addCmd(uint32_t code, const Func<RFDaemonServer, std::vector<uint8_t>, const uint8_t*, uint32_t>& cmd);
private:
    std::vector<uint8_t> parseReceivedData(const std::vector<uint8_t>& data) final;
    AppManager* appMgr = NULL;
    DeviceManager* devMgr = NULL;
    uint8_t streamingDevNum = 0;
    std::fstream* sysImgFile = NULL, * controlFwFile = NULL;
    std::vector<SrvCmd> commands;
};
