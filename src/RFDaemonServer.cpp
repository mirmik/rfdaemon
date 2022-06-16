#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "RFDaemonServer.h"
#include "AppManager.h"
#include "App.h"
#include <nos/fprint.h>

using namespace std;

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	auto make_bind = [&](auto mptr){ return std::bind(
		mptr, this, std::placeholders::_1, std::placeholders::_2); };
	addCmd(GET_APPS_INFO, make_bind(&RFDaemonServer::getAppsInfo));
	addCmd(APPS_START, make_bind(&RFDaemonServer::startAllApps));
	addCmd(APPS_STOP, make_bind(&RFDaemonServer::stopAllApps));
	addCmd(APPS_RESTART, make_bind(&RFDaemonServer::restartAllApps));
	addCmd(GET_CONFIG, make_bind(&RFDaemonServer::getConfig));
	addCmd(SET_CONFIG, make_bind(&RFDaemonServer::setConfig));
	addCmd(UPDATE_IMG, make_bind(&RFDaemonServer::updateSysImg));
	addCmd(UPDATE_FIRMWARE, make_bind(&RFDaemonServer::updateControllerFW));
	addCmd(GET_LOGS, make_bind(&RFDaemonServer::getLogs));
	addCmd(GET_APPS_LIST, make_bind(&RFDaemonServer::getAppsList));
	addCmd(SET_APPS_LIST, make_bind(&RFDaemonServer::setAppsList));
}

RFDaemonServer::~RFDaemonServer()
{
}

void RFDaemonServer::addCmd(uint32_t code, const cmdfunction& cmd)
{
	SrvCmd c;
	c.bitCode = code;
	c.cmd = cmd;
	commands.push_back(c);
}

bool RFDaemonServer::writeFile(const string& filename, const uint8_t* data, uint32_t size)
{
	bool error = false;
	fstream f;
	f.open(filename, fstream::out | fstream::trunc);
	if (f.is_open())
	{
		f.seekp(0);
		f.write((const char*)data, size);
		error = (f.rdstate() & (ios::failbit | ios::badbit)) != 0;
		f.flush();
	}
	return error;
}

vector<uint8_t> RFDaemonServer::startAllApps(const uint8_t*, uint32_t)
{
	appMgr->runApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::stopAllApps(const uint8_t*, uint32_t)
{
	appMgr->closeApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::restartAllApps(const uint8_t*, uint32_t)
{
	appMgr->restartApps();
	return vector<uint8_t>();
}

vector<uint8_t> RFDaemonServer::getConfig(const uint8_t*, uint32_t)
{
	auto settings_path = appMgr->getDeviceDescFilename();
	auto runtime_path = appMgr->getDeviceRuntimeFilename();

	vector<uint8_t> answer(9);
	fstream cfg, runtime;
	cfg.open(settings_path, fstream::in);
	if (cfg.is_open())
	{
		answer[0]++;
		stringstream cfgBuf;
		cfgBuf << cfg.rdbuf();
		string s = cfgBuf.str();
		*(uint32_t*)(answer.data() + 1) = (uint32_t)s.length();
		answer.insert(answer.end(), s.begin(), s.end());
	}
	
	runtime.open(runtime_path, fstream::in);
	if (runtime.is_open())
	{
		answer[0]++;
		stringstream runtimeBuf;
		runtimeBuf << runtime.rdbuf();
		string s = runtimeBuf.str();
		*(uint32_t*)(answer.data() + 5) = (uint32_t)s.length();
		answer.insert(answer.end(), s.begin(), s.end());
	}
	
	return answer;
}

// TODO: Файлы принимаются не абстрактно.
vector<uint8_t> RFDaemonServer::setConfig(const uint8_t* data, uint32_t)
{
	uint8_t fileCount = data[0];
	bool error = false;
	vector<uint8_t> answer(1);
	if (fileCount--)
	{
		uint32_t cfgSize = *(uint32_t*)(data + 1);
		error = writeFile(appMgr->getDeviceDescFilename(), data + 5, cfgSize);
	}
	answer[0] = error;
	return answer;
}

vector<uint8_t> RFDaemonServer::getAppsInfo(const uint8_t*, uint32_t)
{
	struct __attribute__((packed)) AppData
	{
		uint8_t state;
		uint8_t startSuccess;
		int8_t error;
		int32_t pid;
		int64_t uptime;
	};

	uint8_t appCount = (uint8_t)appMgr->getAppCount();
	vector<uint8_t> answer(2 + appCount * sizeof(AppData));
	answer[0] = appCount;

	if (appMgr->errors().size())
	{
		answer[1] = appMgr->errors().front();
		appMgr->errors().pop_front();
	}
	else
		answer[1] = 0;

	AppData* pAppData = (AppData*)(answer.data() + 2);
	auto& apps = appMgr->getAppsList();
	for (int i = 0; i < appCount; i++)
	{
		pAppData[i].state = !apps[i].stopped();
		pAppData[i].startSuccess = true; // Не используется
		pAppData[i].uptime = apps[i].uptime();
		pAppData[i].pid = apps[i].pid();
		if (apps[i].errors().size())
		{
			pAppData[i].error = (int8_t)apps[i].errors().front();
			apps[i].errors().pop();
		}
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::updateSysImg(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	answer[0] = writeFile("new_docker_img.tgz", data, size);
	return answer;
}

vector<uint8_t> RFDaemonServer::updateControllerFW(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	answer[0] = writeFile("new_controller_fw.bin", data, size);
	return answer;
}

vector<uint8_t> RFDaemonServer::setAppsList(const uint8_t* data, uint32_t size)
{
	vector<uint8_t> answer(1);
	answer[0] = writeFile(appMgr->getAppConfigFilename(), data, size);
	if (!answer[0])
	{
		appMgr->loadConfigFile();
		appMgr->restartApps();
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::getAppsList(const uint8_t*, uint32_t)
{
	ifstream f = ifstream(appMgr->getAppConfigFilename(), ifstream::in);
	string s(istreambuf_iterator<char>{f}, {});
	size_t strSize = s.length() + 1;
	vector<uint8_t> answer(strSize);
	memcpy(answer.data(), s.c_str(), strSize);
	return answer;
}

vector<uint8_t> RFDaemonServer::getLogs(const uint8_t*, uint32_t)
{
	size_t namesLen = 0, offset = 0;
	auto logs = appMgr->packLogs();
	for (const auto& l : logs)
		namesLen += l.path.length() + 1;
	size_t logsNum = logs.size();
	vector<uint8_t> answer(1 + (1 + sizeof(uint32_t)) * logsNum + namesLen);
	answer[0] = (uint8_t)logsNum;

	for (size_t i = 0; i < logsNum; i++)
	{
		((uint32_t*)(answer.data() + 1 + logsNum))[i] = (uint32_t)logs[i].data.size();
		size_t pathStrLen = logs[i].path.length() + 1;
		memcpy(answer.data() + 1 + (1 + sizeof(uint32_t)) * logsNum + offset,
			logs[i].path.c_str(), pathStrLen);
		offset += pathStrLen;
		answer.insert(answer.end(), logs[i].data.begin(), logs[i].data.end());
	}
	return answer;
}

void RFDaemonServer::setAppManager(AppManager* manager)
{
	appMgr = manager;
}

vector<uint8_t> RFDaemonServer::parseReceivedData(const vector<uint8_t>& data)
{
	vector<uint8_t> answer;
	bool fromClientToSrv = data[0] != 0;
	uint8_t cmdNum = data[1];
	uint32_t argOffset = 2 + cmdNum * (uint32_t)(sizeof(uint16_t) + sizeof(uint32_t));

	if (fromClientToSrv && (data.size() >= argOffset))
	{
		uint16_t* pCmdList = (uint16_t*)(data.data() + 2);
		uint32_t* pArgSizeList = (uint32_t*)(data.data() + 2 + cmdNum * sizeof(uint16_t));
		answer.resize(2 + cmdNum * (sizeof(uint16_t) + sizeof(uint32_t)));
		answer[0] = 0; // '0' is indicating that this is answer from server to client
		answer[1] = cmdNum; // Number of commands

		for (size_t i = 0; i < cmdNum; i++)
		{
			answer[i * 2 + 2] = (uint8_t)pCmdList[i];
			auto cmdRet = commands[pCmdList[i]].cmd(data.data() + argOffset, pArgSizeList[i]);
			argOffset += pArgSizeList[i];
			*(uint32_t*)(answer.data() + i * sizeof(uint32_t) + 2 + cmdNum * sizeof(uint16_t)) = (uint32_t)cmdRet.size();
			if (cmdRet.size() != 0)
				answer.insert(answer.end(), cmdRet.begin(), cmdRet.end());
		}
	}
	return answer;
}
