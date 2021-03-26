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

float DeviceManager::getMeasuredValue(int devNum)
{
	return measuredValues[devNum];
}

void DeviceManager::setAxisLimits(int devNum, double min, double max)
{
}

void DeviceManager::getAxisLimits(int devNum, double& min, double& max)
{
}

void DeviceManager::setAllAxesToZero()
{
}

void DeviceManager::moveAllAxesToHome()
{
}

const vector<Parameter>& DeviceManager::getParameterList(int devNum)
{
	static vector<Parameter> p;
	return p;
}

void DeviceManager::setParameterList(int devNum, const std::vector<Parameter>& list)
{
}

void DeviceManager::updateFirmware(const fstream& file)
{
}
