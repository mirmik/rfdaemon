#include "RFDaemonServer.h"

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	addCmd(SERVICES_START, Func(this, &RFDaemonServer::startAllApps));
	addCmd(SERVICES_STOP, Func(this, &RFDaemonServer::stopAllApps));
	addCmd(SERVICES_RESTART, Func(this, &RFDaemonServer::restartAllApps));
	addCmd(GET_DEVICES, Func(this, &RFDaemonServer::getAllDevices));
	addCmd(GET_LOGS, Func(this, &RFDaemonServer::getLogs));
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
}

CmdBufResult RFDaemonServer::getAppInfo(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::startAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::stopAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::restartAllApps(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::getAllDevices(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::getLogs(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setStreaming(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setAxesLimits(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::getAxesLimits(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::zeroAllAxes(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::homeAllAxes(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::getStartupScript(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setStartupScript(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::updateSysImg(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::updateControllerFW(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::getParams(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setParams(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setAxisPosToZero(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::homeAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setAxisPos(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::stopAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::jogAxis(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::setAppsList(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::getAppsList(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::beginMultiPacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::multipacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::endMultipacketRead(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::beginMultiPacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::multipacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}

CmdBufResult RFDaemonServer::endMultipacketWrite(uint8_t* rxData, uint8_t* txData, uint32_t remainingRxLen, uint32_t remainingTxLen)
{
	return CmdBufResult();
}
