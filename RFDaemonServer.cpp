#include <fstream>
#include "RFDaemonServer.h"
#include "AppManager.h"
#include "DeviceManager.h"
#include "Axis.h"
#include "App.h"

using namespace std;

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	addCmd(GET_SERVICES_INFO, Func(this, &RFDaemonServer::getAppInfo));
	addCmd(SERVICES_START, Func(this, &RFDaemonServer::startAllApps));
	addCmd(SERVICES_STOP, Func(this, &RFDaemonServer::stopAllApps));
	addCmd(SERVICES_RESTART, Func(this, &RFDaemonServer::restartAllApps));
	addCmd(GET_CONFIG, Func(this, &RFDaemonServer::getConfig));
	addCmd(SET_CONFIG, Func(this, &RFDaemonServer::setConfig));
	addCmd(GET_DEV_LOGS, Func(this, &RFDaemonServer::getDevErrLogs));
	addCmd(GET_MEASUREMENTS, Func(this, &RFDaemonServer::getSystemMeasurements));
	addCmd(AXES_LIMITS_SET, Func(this, &RFDaemonServer::setAxesLimits));
	addCmd(UPDATE_IMG, Func(this, &RFDaemonServer::updateSysImg));
	addCmd(UPDATE_FIRMWARE, Func(this, &RFDaemonServer::updateControllerFW));
	addCmd(PARAMS_READ, Func(this, &RFDaemonServer::getParams));
	addCmd(PARAMS_WRITE, Func(this, &RFDaemonServer::setParams));
	addCmd(AXIS_SETZERO, Func(this, &RFDaemonServer::setAxisPosToZero));
	addCmd(AXIS_HOME, Func(this, &RFDaemonServer::homeAxis));
	addCmd(AXIS_SETPOS, Func(this, &RFDaemonServer::setAxisPos));
	addCmd(AXIS_STOP, Func(this, &RFDaemonServer::stopAxis));
	addCmd(AXIS_JOG, Func(this, &RFDaemonServer::jogAxis));
	addCmd(SET_RUNNABLE_SERVICES_LIST, Func(this, &RFDaemonServer::setAppsList));
	addCmd(GET_RUNNABLE_SERVICES_LIST, Func(this, &RFDaemonServer::getAppsList));
	addCmd(GET_SERVICES_LOGS, Func(this, &RFDaemonServer::getAppLogs));
}

void RFDaemonServer::addCmd(uint32_t code, const Func<RFDaemonServer, vector<uint8_t>, const uint8_t*, uint32_t>& cmd)
{
	SrvCmd c;
	c.bitCode = code;
	c.cmd = cmd;
	commands.push_back(c);
}

vector<uint8_t> RFDaemonServer::getAppInfo(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	answer[0] = appMgr->getAppCount();

	const vector<App>& apps = appMgr->getAppsList();

	for (const auto& a : apps)
	{
		uint8_t arr[13];
		arr[0] = !a.stopped();
		*(int32_t*)(arr + 1) = a.pid();
		*(uint64_t*)(arr + 5) = a.uptime();
		answer.insert(answer.end(), arr, arr + sizeof(arr));
		answer.insert(answer.end(), a.name().c_str(), a.name().c_str() + a.name().length() + 1);
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::startAllApps(const uint8_t* data, uint32_t size)
{
	appMgr->runApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::stopAllApps(const uint8_t* data, uint32_t size)
{
	appMgr->closeApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::restartAllApps(const uint8_t* data, uint32_t size)
{
	appMgr->restartApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::getConfig(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(4);
	vector<uint8_t> file = devMgr->getDevDescFileRaw();
	*((uint32_t*)answer.data()) = file.size();
	answer.insert(answer.end(), file.begin(), file.end());
	return answer;
}

vector<uint8_t> RFDaemonServer::setConfig(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	answer[0] = devMgr->updateDevDescFile((const char*)data + 4, size - 4);
	return answer;
}

vector<uint8_t> RFDaemonServer::getDevErrLogs(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer;
	const char* filePtr = devMgr->getLogFile().c_str();
	answer.assign(filePtr, filePtr + devMgr->getLogFile().length() + 1);
	return answer;
}

vector<uint8_t> RFDaemonServer::getSystemMeasurements(const uint8_t* data, uint32_t size)
{
	int appCount = appMgr->getAppCount();
	vector<uint8_t> answer(2 + sizeof(double) * 2 + (sizeof(int64_t) + 1) * appCount * 2 + 1);
	uint8_t devNum = data[0];
	uint8_t axisNum = data[1];
	answer[0] = data[0];
	answer[1] = data[1];
	double* pData = (double*)(answer.data() + 2);
	uint8_t* pAppCount = answer.data() + 2 + sizeof(double) * 2;
	uint8_t* pAppState = answer.data() + 2 + sizeof(double) * 2 + 1;
	uint8_t* pAppStartSuccess = answer.data() + 2 + sizeof(double) * 2 + appCount + 1;
	int64_t* pUptime = (int64_t*)(answer.data() + 2 + sizeof(double) * 2 + appCount * 2 + 1);
	if (devNum < devMgr->devCount())
		pData[0] = devMgr->getDevList()[devNum].sensorValue();
	else
		pData[0] = 0;
	if (axisNum < devMgr->axesCount())
		pData[1] = devMgr->getAxesList()[axisNum].position();
	else
		pData[1] = 0;

	pAppCount[0] = appCount;

	for (int i = 0; i < appCount; i++)
	{
		pAppState[i] = !appMgr->getAppsList()[i].stopped();
		pAppStartSuccess[i] = appMgr->getAppsList()[i].successfulStart();
		pUptime[i] = appMgr->getAppsList()[i].uptime();
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::setAxesLimits(const uint8_t* data, uint32_t size)
{
	size_t devCount = std::min<size_t>(data[0], devMgr->devCount());
	double* value = (double*)(data + 1);
	for (size_t i = 0; i < devCount; i++)
		devMgr->writeAxisLimits(i, value[i * 2], value[i * 2 + 1]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::updateSysImg(const uint8_t* data, uint32_t size)
{
	auto f = new fstream("new_docker_img.tgz", fstream::out);
	if (f->is_open())
	{
		f->write((const char*)data, size);
		f->flush();
		f->close();
	}
	delete f;
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::updateControllerFW(const uint8_t* data, uint32_t size)
{
	auto f = new fstream("new_controller_fw.bin", fstream::out);
	if (f->is_open())
	{
		f->write((const char*)data, size);
		f->flush();
		devMgr->updateFirmware(*f);
		f->close();
	}
	delete f;
	return vector<uint8_t>();
}

/*
Request (>6 bytes):
[0] - device id
[1-2] - requested number of parameters
[3-x] - names of parameters

Answer (>16 bytes):
[0] - device id
[1-2] - requested number of parameters
[3-4] - actual number of read parameters
[5-y] - values of read parameters
[y-z] - names of read parameters
*/
vector<uint8_t> RFDaemonServer::getParams(const uint8_t* data, uint32_t size)
{
	uint8_t devId = data[0];
	uint16_t requestedParamCount = *(uint16_t*)(data + 1);
	vector<uint8_t> answer(5);
	answer.reserve(answer.size() + requestedParamCount * (sizeof(double) + 5));
	answer[0] = devId;
	*(uint16_t*)(answer.data() + 1) = requestedParamCount;
	vector<string> paramNames;
	paramNames.reserve(requestedParamCount * 5);
	const uint8_t* pName = data + 3;

	string paramName;
	size_t paramsRead = 0;

	size_t len;
	for (size_t i = 0; i < requestedParamCount; i++)
	{
		len = strnlen((char*)pName, 5);
		paramName = string((char*)pName, len);
		
		bool success;
		double value = devMgr->readParameterValue(devId, paramName, success);
		if (success)
		{
			paramNames.push_back(paramName);
			answer.insert(answer.end(), (uint8_t*)&value, (uint8_t*)&value + sizeof(double));
			paramsRead++;
		}
		pName += len + 1;
	}	
	*(uint16_t*)(answer.data() + 3) = paramsRead;
	for (size_t i = 0; i < paramsRead; i++)
		answer.insert(answer.end(), paramNames[i].c_str(), paramNames[i].c_str() + paramNames[i].length() + 1);
	return answer;
}

/*
Request (>12 bytes):
[0] - device id
[1-2] - requested number of parameters
[3-x] - values of parameters
[x-y] - names of parameters

Answer (5 bytes):
[0] - device id
[1-2] - requested number of parameters
[3-4] - actual number of written parameters
*/
vector<uint8_t> RFDaemonServer::setParams(const uint8_t* data, uint32_t size)
{
	uint8_t devId = data[0];
	vector<uint8_t> answer(5);
	if (devId < devMgr->devCount())
	{
		uint16_t requestedParamCount = *(uint16_t*)(data + 1);
		answer[0] = devId;
		*(uint16_t*)(answer.data() + 1) = requestedParamCount;
		vector<string> paramNames;
		vector<double> paramValues;
		double* pValue = (double*)data + 3;
		const uint8_t* pName = (uint8_t*)pValue + sizeof(double) * requestedParamCount;

		size_t len;
		for (size_t i = 0; i < requestedParamCount; i++)
		{
			len = strnlen((char*)pName, 5);
			paramNames.push_back(string((char*)pName, len));
			paramValues.push_back(pValue[i]);
			pName += len + 1;
		}

		size_t paramsWritten = devMgr->writeParameterValues(devId, paramNames, paramValues);
		*(uint16_t*)(answer.data() + 3) = paramsWritten;
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::setAxisPosToZero(const uint8_t* data, uint32_t size)
{
	if (data[0] == 0xFF)
		devMgr->setAllAxesToZero();
	else
		devMgr->setAxisToZero(data[0]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::homeAxis(const uint8_t* data, uint32_t size)
{
	if (data[0] == 0xFF)
		devMgr->moveAllAxesToHome();
	else
		devMgr->moveAxisToHome(data[0]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::setAxisPos(const uint8_t* data, uint32_t size)
{
	devMgr->setAxisAbsPosition(data[0], *(double*)(data + 1));
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::stopAxis(const uint8_t* data, uint32_t size)
{
	if (data[0] == 0xFF)
		devMgr->stopAllAxes();
	else
		devMgr->stopAxis(data[0]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::jogAxis(const uint8_t* data, uint32_t size)
{
	devMgr->jogAxis(data[0], *(double*)(data + 1));
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::setAppsList(const uint8_t* data, uint32_t size)
{
	string s = string((char*)data, size);
	appMgr->updateConfigFile(s);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::getAppsList(const uint8_t* data, uint32_t size)
{
	ifstream& f = appMgr->getAppConfigFile();
	string s(istreambuf_iterator<char>{f}, {});
	uint32_t strSize = s.length() + 1;
	vector<uint8_t> answer(strSize);
	memcpy(answer.data(), s.c_str(), strSize);
	return answer;
}

vector<uint8_t> RFDaemonServer::getAppLogs(const uint8_t* data, uint32_t size)
{
	ifstream& f = appMgr->getLogFile();
	string s(istreambuf_iterator<char>{f}, {});
	uint32_t strSize = s.length() + 1;
	vector<uint8_t> answer(strSize);
	memcpy(answer.data(), s.c_str(), strSize);
	return answer;
}

void RFDaemonServer::setAppManager(AppManager* manager)
{
	appMgr = manager;
}

void RFDaemonServer::setDeviceManager(DeviceManager* manager)
{
	devMgr = manager;
}

vector<uint8_t> RFDaemonServer::parseReceivedData(const vector<uint8_t>& data)
{
	vector<uint8_t> answer;
	bool fromClientToSrv = data[0] != 0;
	uint8_t cmdNum = data[1];
	uint32_t argOffset = 2 + cmdNum * (sizeof(uint16_t) + sizeof(uint32_t));

	if (fromClientToSrv && (data.size() >= argOffset))
	{
		uint16_t* pCmdList = (uint16_t*)(data.data() + 2);
		uint32_t* pArgSizeList = (uint32_t*)(data.data() + 2 + cmdNum * sizeof(uint16_t));
		answer.resize(2 + cmdNum * (sizeof(uint16_t) + sizeof(uint32_t)));
		answer[0] = 0; // '0' is indicating that this is answer from server to client
		answer[1] = cmdNum; // Number of commands

		for (size_t i = 0; i < cmdNum; i++)
		{
			answer[i * 2 + 2] = pCmdList[i];
			auto cmdRet = commands[pCmdList[i]].cmd(data.data() + argOffset, pArgSizeList[i]);
			argOffset += pArgSizeList[i];
			*(uint32_t*)(answer.data() + i * sizeof(uint32_t) + 2 + cmdNum * sizeof(uint16_t)) = cmdRet.size();
			if (cmdRet.size() != 0)
				answer.insert(answer.end(), cmdRet.begin(), cmdRet.end());
		}
	}
	return answer;
}
