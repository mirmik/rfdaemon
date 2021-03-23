#pragma once

#include "TcpClient.h"
#include <vector>
#include <string>

class DeviceManager : public TcpClient
{
public:
	DeviceManager(uint16_t port);
	const std::vector<std::string>& getDevList() const;
	const std::string& getLogFile() const;
private:
	std::vector<std::string> devices;
	std::string logFileStr;
};
