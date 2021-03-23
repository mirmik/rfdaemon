#include "RFDaemonServer.h"
#include "AppManager.h"
#include "DeviceManager.h"

using namespace std;

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	addCmd(SERVICES_START, Func(this, &RFDaemonServer::startAllApps));
	addCmd(SERVICES_STOP, Func(this, &RFDaemonServer::stopAllApps));
	addCmd(SERVICES_RESTART, Func(this, &RFDaemonServer::restartAllApps));
	addCmd(GET_DEVICES, Func(this, &RFDaemonServer::getAllDevices));
	addCmd(GET_DEV_LOGS, Func(this, &RFDaemonServer::getDevErrLogs));
	addCmd(SET_STREAMING, Func(this, &RFDaemonServer::setStreaming));
	addCmd(AXES_LIMITS_SET, Func(this, &RFDaemonServer::setAxesLimits));
	addCmd(AXES_LIMITS_GET, Func(this, &RFDaemonServer::getAxesLimits));
	addCmd(ZERO_ALL_AXES, Func(this, &RFDaemonServer::zeroAllAxes));
	addCmd(HOME_ALL_AXES, Func(this, &RFDaemonServer::homeAllAxes));
	addCmd(STARTUP_SCRIPT_GET, Func(this, &RFDaemonServer::getStartupScript));
	addCmd(STARTUP_SCRIPT_SET, Func(this, &RFDaemonServer::setStartupScript));
	addCmd(UPDATE_IMG, Func(this, &RFDaemonServer::updateSysImg));
	addCmd(UPDATE_FIRMWARE, Func(this, &RFDaemonServer::updateControllerFW));
	addCmd(PARAMS_READ, Func(this, &RFDaemonServer::getParams));
	addCmd(PARAMS_WRITE, Func(this, &RFDaemonServer::setParams));
	addCmd(AXIS_SETZERO, Func(this, &RFDaemonServer::setAxisPosToZero));
	addCmd(AXIS_HOME, Func(this, &RFDaemonServer::homeAxis));
	addCmd(AXIS_SETPOS, Func(this, &RFDaemonServer::setAxisPos));
	addCmd(AXIS_STOP, Func(this, &RFDaemonServer::stopAxis));
	addCmd(AXIS_JOG, Func(this, &RFDaemonServer::jogAxis));
	addCmd(SET_RUNNABLE_APPS_LIST, Func(this, &RFDaemonServer::setAppsList));
	addCmd(GET_RUNNABLE_APPS_LIST, Func(this, &RFDaemonServer::getAppsList));
	addCmd(BEGIN_MULTIPACKET_READ, Func(this, &RFDaemonServer::beginMultiPacketRead));
	addCmd(MULTIPACKET_DATA_READ, Func(this, &RFDaemonServer::multipacketRead));
	addCmd(END_MULTIPACKET_READ, Func(this, &RFDaemonServer::endMultipacketRead));
	addCmd(BEGIN_MULTIPACKET_WRITE, Func(this, &RFDaemonServer::beginMultiPacketWrite));
	addCmd(MULTIPACKET_DATA_WRITE, Func(this, &RFDaemonServer::multipacketWrite));
	addCmd(END_MULTIPACKET_WRITE, Func(this, &RFDaemonServer::endMultipacketWrite));
	addCmd(GET_APP_LOGS, Func(this, &RFDaemonServer::getAppLogs));
}

CmdBufResult RFDaemonServer::getAppInfo(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	vector<uint32_t> appNameLen;
	uint32_t overallLen = 2;
	uint8_t* start = txData;
	bool appCountTooHigh = false;

	for (auto& a : appMgr->getAppNames())
	{
		uint32_t len = a.length() + 1;
		if (remainingTxLen >= (overallLen + len + 4))
		{
			appNameLen.push_back(len);
			overallLen += len + 4;
		}
	}

	int16_t appCount = appNameLen.size();
	if (appMgr->getAppNames().size() > (size_t)appCount)
		appCountTooHigh = true;

	if (appCount)
	{
		*(int16_t*)txData = appCountTooHigh ? -appCount : appCount;
		txData += 2;
		for (int16_t i = 0; i < appCount; i++)
			txData++[i] = appMgr->getAppStatusList()[i];

		for (int16_t i = 0; i < appCount; i++)
		{
			*(uint32_t*)(txData) = appMgr->getAppPids()[i];
			txData += 4;
		}

		for (int16_t i = 0; i < appCount; i++)
		{
			memcpy(txData, appMgr->getAppNames()[i].c_str(), appNameLen[i]);
			txData += appNameLen[i];
		}
	}
	return CmdBufResult{ 0, (uint32_t)(txData - start) };
}

CmdBufResult RFDaemonServer::startAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	if (!appMgr->appsIsRunning())
		appMgr->runApps();
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::stopAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	if (appMgr->appsIsRunning())
		appMgr->closeApps();
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::restartAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	appMgr->restartApps();
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::getAllDevices(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	uint8_t* start = txData;
	uint32_t overallLen = 1;
	vector<int> devNameLen;
	bool devCountTooHigh = false;
	
	for (auto& d : devMgr->getDevList())
	{
		uint32_t len = d.length() + 1;
		if (remainingTxLen >= (overallLen + len))
		{
			devNameLen.push_back(len);
			overallLen += len;
		}
	}

	int devCount = devNameLen.size();
	if (devMgr->getDevList().size() > (size_t)devCount)
		devCountTooHigh = true;

	if (devCount)
	{
		*(int16_t*)txData = devCountTooHigh ? -devCount : devCount;
		txData += 2;

		for (int16_t i = 0; i < devCount; i++)
		{
			memcpy(txData, devMgr->getDevList()[i].c_str(), devNameLen[i]);
			txData += devNameLen[i];
		}
	}

	return CmdBufResult{ 0, (uint32_t)(txData - start) };
}

CmdBufResult RFDaemonServer::getDevErrLogs(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	uint32_t copyLength = std::min<uint32_t>(devMgr->getLogFile().length() + 1, remainingTxLen);
	memcpy(txData, devMgr->getLogFile().c_str(), copyLength);
	return CmdBufResult{ 0, copyLength };
}

CmdBufResult RFDaemonServer::setStreaming(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{

	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::setAxesLimits(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::getAxesLimits(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::zeroAllAxes(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::homeAllAxes(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::getStartupScript(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::setStartupScript(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::updateSysImg(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::updateControllerFW(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::getParams(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::setParams(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::setAxisPosToZero(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::homeAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::setAxisPos(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::stopAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::jogAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::setAppsList(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::getAppsList(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::beginMultiPacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::multipacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::endMultipacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::beginMultiPacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::multipacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::endMultipacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult{ 0, 0 };
}

CmdBufResult RFDaemonServer::getAppLogs(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	uint32_t copyLength = std::min<uint32_t>(appMgr->getLogFile().length() + 1, remainingTxLen);
	memcpy(txData, appMgr->getLogFile().c_str(), copyLength);
	return CmdBufResult{ 0, copyLength };
}

void RFDaemonServer::setAppManager(AppManager* manager)
{
	appMgr = manager;
}

void RFDaemonServer::setDeviceManager(DeviceManager* manager)
{
	devMgr = manager;
}
