#pragma once
#include "TcpServer.h"
#include <vector>
#include "functors.h"

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
        GET_APPS_INFO = 0,
        APPS_START,
        APPS_STOP,
        APPS_RESTART,
        GET_CONFIG,
        SET_CONFIG,
        UPDATE_IMG,
        UPDATE_FIRMWARE,
        GET_APPS_LOGS,
        GET_APPS_LIST,
        SET_APPS_LIST
    } CmdId;
    RFDaemonServer(uint16_t port);
    std::vector<uint8_t> getAppsInfo(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> startAllApps(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> stopAllApps(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> restartAllApps(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getConfig(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setConfig(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> updateSysImg(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> updateControllerFW(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getAppLogs(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> getAppsList(const uint8_t* data, uint32_t size);
    std::vector<uint8_t> setAppsList(const uint8_t* data, uint32_t size);
    void setAppManager(AppManager* manager);
    void addCmd(uint32_t code, const Func<RFDaemonServer, std::vector<uint8_t>, const uint8_t*, uint32_t>& cmd);
private:
    bool writeFile(const std::string& filename, const uint8_t* data, uint32_t size);
    std::vector<uint8_t> parseReceivedData(const std::vector<uint8_t>& data) final;
    AppManager* appMgr = NULL;
    std::vector<SrvCmd> commands;
};
