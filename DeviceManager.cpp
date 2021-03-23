#include "DeviceManager.h"

using namespace std;

DeviceManager::DeviceManager(uint16_t port) : TcpClient(port)
{

}

const vector<string>& DeviceManager::getDevList() const
{
	return devices;
}

const std::string& DeviceManager::getLogFile() const
{
	return logFileStr;
}
