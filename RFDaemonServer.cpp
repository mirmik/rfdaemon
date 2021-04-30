#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "RFDaemonServer.h"
#include "AppManager.h"
#include "App.h"

using namespace std;

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
	addCmd(GET_APPS_INFO, Func(this, &RFDaemonServer::getAppsInfo));
	addCmd(APPS_START, Func(this, &RFDaemonServer::startAllApps));
	addCmd(APPS_STOP, Func(this, &RFDaemonServer::stopAllApps));
	addCmd(APPS_RESTART, Func(this, &RFDaemonServer::restartAllApps));
	addCmd(GET_CONFIG, Func(this, &RFDaemonServer::getConfig));
	addCmd(SET_CONFIG, Func(this, &RFDaemonServer::setConfig));
	addCmd(UPDATE_IMG, Func(this, &RFDaemonServer::updateSysImg));
	addCmd(UPDATE_FIRMWARE, Func(this, &RFDaemonServer::updateControllerFW));
	addCmd(GET_APPS_LOGS, Func(this, &RFDaemonServer::getAppLogs));
	addCmd(GET_APPS_LIST, Func(this, &RFDaemonServer::getAppsList));
	addCmd(SET_APPS_LIST, Func(this, &RFDaemonServer::setAppsList));
}

void RFDaemonServer::addCmd(uint32_t code, const Func<RFDaemonServer, vector<uint8_t>, const uint8_t*, uint32_t>& cmd)
{
	SrvCmd c;
	c.bitCode = code;
	c.cmd = cmd;
	commands.push_back(c);
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

	fstream f;
	f.open(appMgr->getDeviceDescFilename(), fstream::in);
	if (f.is_open())
	{
		stringstream buffer;
		buffer << f.rdbuf();
		string s = buffer.str();
		*(int*)answer.data() = s.length() + 1;
		answer.insert(answer.end(), s.begin(), s.end() + 1);
		f.close();
	}
	return answer;
}

vector<uint8_t> RFDaemonServer::setConfig(const uint8_t* data, uint32_t size)
{
	bool error = false;
	vector<uint8_t> answer(1);
	fstream f;
	f.open(appMgr->getDeviceDescFilename(), fstream::out | fstream::trunc);
	if (f.is_open())
	{
		f.seekp(0);
		f.write((char*)data, size);
		error = (f.rdstate() & (ios::failbit | ios::badbit)) != 0;
		f.flush();
		f.close();
	}
	answer[0] = error;
	return answer;
}

vector<uint8_t> RFDaemonServer::getAppsInfo(const uint8_t* data, uint32_t size)
{
	struct __attribute__((packed)) AppData
	{
		uint8_t state;
		uint8_t startSuccess;
		uint8_t nameLength;
		int8_t error;
		int32_t pid;
		int64_t uptime;
	};

	uint8_t appCount = appMgr->getAppCount();
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
		pAppData[i].startSuccess = apps[i].successfulStart();
		pAppData[i].uptime = apps[i].uptime();
		pAppData[i].pid = apps[i].pid();
		if (apps[i].errors().size())
		{
			pAppData[i].error = apps[i].errors().front();
			apps[i].errors().pop();
		}
		answer.insert(answer.end(), apps[i].name().c_str(), apps[i].name().c_str() + apps[i].name().length() + 1);
	}
	return answer;
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
		f->close();
	}
	delete f;
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
