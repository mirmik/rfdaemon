#include "RFDaemonServer.h"
#include "AppManager.h"
#include "DeviceManager.h"
#include "Axis.h"
#include <fstream>

using namespace std;

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	addCmd(GET_SERVICES_INFO, Func(this, &RFDaemonServer::getAppInfo));
	addCmd(SERVICES_START, Func(this, &RFDaemonServer::startAllApps));
	addCmd(SERVICES_STOP, Func(this, &RFDaemonServer::stopAllApps));
	addCmd(SERVICES_RESTART, Func(this, &RFDaemonServer::restartAllApps));
	addCmd(GET_DEVICES, Func(this, &RFDaemonServer::getAllDevices));
	addCmd(GET_AXES, Func(this, &RFDaemonServer::getAllAxes));
	addCmd(GET_DEV_LOGS, Func(this, &RFDaemonServer::getDevErrLogs));
	addCmd(GET_MEASUREMENTS, Func(this, &RFDaemonServer::getDevSensorValues));
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

	vector<uint64_t> uptimes = appMgr->getAppUptimeList();

	for (size_t i = 0; i < appMgr->getAppCount(); i++)
	{
		uint8_t arr[13];
		arr[0] = appMgr->getAppStatusList()[i];
		*(int32_t*)(arr + 1) = appMgr->getAppPids()[i];
		*(uint64_t*)(arr + 5) = uptimes[i];
		answer.insert(answer.end(), arr, arr + sizeof(arr));
		const string& appName = appMgr->getAppNames()[i];
		answer.insert(answer.end(), appName.c_str(), appName.c_str() + appName.length() + 1);
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::startAllApps(const uint8_t* data, uint32_t size)
{
	if (!appMgr->appsIsRunning())
		appMgr->runApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::stopAllApps(const uint8_t* data, uint32_t size)
{
	if (appMgr->appsIsRunning())
		appMgr->closeApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::restartAllApps(const uint8_t* data, uint32_t size)
{
	appMgr->restartApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::getAllDevices(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	const vector<Device>& devList = devMgr->getDevList();
	answer[0] = devMgr->devCount();
	for (auto& d : devList)
	{
		answer.push_back((unsigned char)d.type());
		const string& name = d.name();
		answer.insert(answer.end(), name.c_str(), name.c_str() + name.length() + 1);
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::getAllAxes(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	const vector<Axis>& axesList = devMgr->getAxesList();
	answer[0] = axesList.size();
	for (auto& ax : axesList)
	{
		answer.push_back((unsigned char)ax.type());
		answer.push_back((unsigned char)ax.motionType());
		double limits[2] = {ax.minLimit(), ax.maxLimit() };
		answer.insert(answer.end(), (char*)limits, (char*)limits + sizeof(limits));
		const string& name = ax.name();
		answer.insert(answer.end(), name.c_str(), name.c_str() + name.length() + 1);
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::getDevErrLogs(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer;
	const char* filePtr = devMgr->getLogFile().c_str();
	answer.assign(filePtr, filePtr + devMgr->getLogFile().length() + 1);
	return answer;
}

vector<uint8_t> RFDaemonServer::getDevSensorValues(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(2 + sizeof(double) * 2);
	uint8_t devNum = data[0];
	uint8_t axisNum = data[1];
	answer[0] = data[0];
	answer[1] = data[1];
	double* pData = (double*)(answer.data() + 2);
	if (devNum < devMgr->devCount())
		pData[0] = devMgr->getDevList()[devNum].sensorValue();
	else
		pData[0] = 0;
	if (axisNum < devMgr->axesCount())
		pData[1] = devMgr->getAxesList()[axisNum].position();
	else
		pData[1] = 0;
	return answer;
}

vector<uint8_t> RFDaemonServer::setAxesLimits(const uint8_t* data, uint32_t size)
{
	size_t devCount = std::min<size_t>(data[0], devMgr->devCount());
	double* value = (double*)(data + 1);
	for (size_t i = 0; i < devCount; i++)
		devMgr->setAxisLimits(i, value[i * 2], value[i * 2 + 1]);
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

vector<uint8_t> RFDaemonServer::getParams(const uint8_t* data, uint32_t size)
{
	uint8_t devId = data[0];
	bool getAllParameters = data[1] != 0;
	const vector<Parameter>& parameters = devMgr->getParameterList(devId);
	uint16_t requestedParamCount = parameters.size();
	size_t startParamId = 0;

	vector<uint8_t> answer(4);
	answer.reserve(answer.size() + parameters.size() * (sizeof(uint16_t) + sizeof(Parameter)));
	answer[0] = devId;
	answer[1] = getAllParameters;
	uint16_t* pParamNumAnsList = NULL, *pParamNumList = NULL;
	
	if (getAllParameters)
		*(uint16_t*)(answer.data() + 2) = parameters.size();
	else
	{
		requestedParamCount = *(uint16_t*)(data + 2);
		*(uint16_t*)(answer.data() + 2) = requestedParamCount;

		if (requestedParamCount <= parameters.size())
		{
			pParamNumList = (uint16_t*)(data + 4);
			pParamNumAnsList = (uint16_t*)(answer.data() + 4);
			startParamId = pParamNumList[0];
			answer.resize(answer.size() + requestedParamCount * sizeof(uint16_t));
			for (size_t i = 0; i < requestedParamCount; i++)
				pParamNumAnsList[i] = pParamNumList[i];
		}
	}

	for (size_t i = startParamId; (i < parameters.size()) && (i < requestedParamCount); i++)
	{
		bool include = getAllParameters ? true : (pParamNumAnsList[i] == i);
		if (include)
		{
			const string& name = parameters[i].name();
			const string& desc = parameters[i].description();
			const string& unit = parameters[i].unit();
			double vals[] = { parameters[i].getValue(), parameters[i].getRangeMin(), parameters[i].getRangeMax() };

			answer.insert(answer.end(), name.c_str(), name.c_str() + name.length() + 1);
			answer.insert(answer.end(), desc.c_str(), desc.c_str() + desc.length() + 1);
			answer.insert(answer.end(), unit.c_str(), unit.c_str() + unit.length() + 1);
			answer.push_back((unsigned char)(parameters[i].getType()));
			answer.insert(answer.end(), (char*)vals, (char*)vals + sizeof(vals));
		}
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::setParams(const uint8_t* data, uint32_t size)
{
	uint8_t devId = data[0];
	bool setAllParameters = data[1] != 0;
	size_t startParamId = 0;
	size_t devParamCount = devMgr->getParameterList(devId).size();
	uint16_t requestedParamCount = devParamCount;
	vector<uint8_t> answer(4);
	answer[0] = devId;
	answer[1] = setAllParameters;
	uint16_t* pParamNumList = NULL;
	vector<uint16_t> paramIdList;
	vector<double> paramValList;
	double* pValList = (double*)(data + 4);

	if (!setAllParameters)
	{
		pParamNumList = (uint16_t*)(data + 4);
		requestedParamCount = *(uint16_t*)(data + 2);
		pValList = (double*)(data + 4 + requestedParamCount * sizeof(uint16_t));
		startParamId = pParamNumList[0];
	}

	for (size_t i = startParamId, j = 0; (i < devParamCount) && (j < requestedParamCount); i++)
	{
		if (setAllParameters ? true : (pParamNumList[j] == i))
		{
			paramIdList.push_back(i);
			paramValList.push_back(pValList[j]);
			j++;
		}
	}
	if (setAllParameters)
		devMgr->setParameterValues(devId, paramValList);
	else
		devMgr->setParameterValues(devId, paramIdList, paramValList);
	
	*(uint16_t*)(answer.data() + 2) = paramValList.size();
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
