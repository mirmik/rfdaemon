#include "RFDaemonServer.h"
#include "AppManager.h"
#include "DeviceManager.h"
#include <fstream>

using namespace std;

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	addCmd(SERVICES_START, Func(this, &RFDaemonServer::startAllApps));
	addCmd(SERVICES_STOP, Func(this, &RFDaemonServer::stopAllApps));
	addCmd(SERVICES_RESTART, Func(this, &RFDaemonServer::restartAllApps));
	addCmd(GET_DEVICES, Func(this, &RFDaemonServer::getAllDevices));
	addCmd(GET_DEV_LOGS, Func(this, &RFDaemonServer::getDevErrLogs));
	addCmd(GET_CURR_MEAS_VALUE, Func(this, &RFDaemonServer::getCurrentDevValue));
	addCmd(AXES_LIMITS_SET, Func(this, &RFDaemonServer::setAxesLimits));
	addCmd(AXES_LIMITS_GET, Func(this, &RFDaemonServer::getAxesLimits));
	addCmd(ZERO_ALL_AXES, Func(this, &RFDaemonServer::zeroAllAxes));
	addCmd(HOME_ALL_AXES, Func(this, &RFDaemonServer::homeAllAxes));
	addCmd(STARTUP_SCRIPT_GET, Func(this, &RFDaemonServer::getStartupScript)); //Not need yet
	addCmd(STARTUP_SCRIPT_SET, Func(this, &RFDaemonServer::setStartupScript)); //Not need yet
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
	addCmd(GET_APP_LOGS, Func(this, &RFDaemonServer::getAppLogs));
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

	for (size_t i = 0; i < appMgr->getAppCount(); i++)
	{
		uint8_t arr[5];
		arr[0] = appMgr->getAppStatusList()[i];
		*(pid_t*)(arr + 1) = appMgr->getAppPids()[i];
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
	vector<string> devNamesList = devMgr->getDevList();
	answer[0] = devMgr->getDevCount();
	for (auto& d : devNamesList)
		answer.insert(answer.end(), d.c_str(), d.c_str() + d.length() + 1);
	return answer;
}

vector<uint8_t> RFDaemonServer::getDevErrLogs(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer;
	const char* filePtr = devMgr->getLogFile().c_str();
	answer.assign(filePtr, filePtr + devMgr->getLogFile().length() + 1);
	return answer;
}

vector<uint8_t> RFDaemonServer::getCurrentDevValue(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(5);
	answer[0] = data[0];
	*(float*)(answer.data() + 1) = devMgr->getMeasuredValue(answer[0]);
	return answer;
}

vector<uint8_t> RFDaemonServer::setAxesLimits(const uint8_t* data, uint32_t size)
{
	size_t devCount = std::min<size_t>(data[0], devMgr->getDevCount());
	double* value = (double*)(data + 1);
	for (size_t i = 0; i < devCount; i++)
		devMgr->setAxisLimits(i, value[i * 2], value[i * 2 + 1]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::getAxesLimits(const uint8_t* data, uint32_t size)
{
	size_t devCount = std::min<size_t>(data[0], devMgr->getDevCount());
	vector<uint8_t> answer(1 + 2 * devCount * sizeof(double));
	answer[0] = devCount;
	double* value = (double*)(answer.data() + 1);
	for (size_t i = 0; i < devCount; i++)
		devMgr->getAxisLimits(i, value[i * 2], value[i * 2 + 1]);
	return answer;
}

vector<uint8_t> RFDaemonServer::zeroAllAxes(const uint8_t* data, uint32_t size)
{
	devMgr->setAllAxesToZero();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::homeAllAxes(const uint8_t* data, uint32_t size)
{
	devMgr->moveAllAxesToHome();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::getStartupScript(const uint8_t* data, uint32_t size)
{
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::setStartupScript(const uint8_t* data, uint32_t size)
{
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::updateSysImg(const uint8_t* data, uint32_t size)
{
	auto f = new fstream("new_docker_img.tgz");
	f->write((const char*)data, size);
	f->flush();
	f->close();
	delete f;
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::updateControllerFW(const uint8_t* data, uint32_t size)
{
	auto f = new fstream("new_controller_fw.bin");
	f->write((const char*)data, size);
	f->flush();
	f->close();
	devMgr->updateFirmware(*f);
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
			answer.insert(answer.end(), vals, vals + 2);
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
	}

	for (size_t i = startParamId; (i < devParamCount) && (i < requestedParamCount); i++)
	{
		bool include = setAllParameters ? true : (pParamNumList[i] == i);
		if (include)
		{
			paramIdList.push_back(i);
			paramValList.push_back(*pValList);
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
	devMgr->setDevAxisToZero(data[0]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::homeAxis(const uint8_t* data, uint32_t size)
{
	devMgr->moveDevAxisToHome(data[0]);
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::setAxisPos(const uint8_t* data, uint32_t size)
{
	devMgr->setAxisPosition(data[0], *(double*)(data + 1));
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::stopAxis(const uint8_t* data, uint32_t size)
{
	devMgr->stopDevAxis(data[0]);
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

	if (fromClientToSrv && (data.size() > (cmdNum * (1 + sizeof(uint32_t)))))
	{
		uint8_t* pCmdList = (uint8_t*)(data.data() + 2);
		uint32_t* pArgSizeList = (uint32_t*)(data.data() + 2 + cmdNum);
		uint32_t dataOffset = 0;

		answer.resize(2 + cmdNum * (1 + sizeof(uint32_t)));
		answer[0] = cmdNum; // Number of commands
		answer[1] = 0; // '0' is indicating that this is answer from server to client

		for (size_t i = 0; i < cmdNum; i++)
		{
			answer[i + 2] = pCmdList[i];
			auto cmdRet = commands[pCmdList[i]].cmd(data.data() + dataOffset, pArgSizeList[i]);
			dataOffset += pArgSizeList[i];
			*(uint32_t*)(answer.data() + i * 4 + 2 + cmdNum) = cmdRet.size();
			if (cmdRet.size() != 0)
				answer.insert(answer.end(), cmdRet.begin(), cmdRet.end());
		}
	}
	return answer;
}
