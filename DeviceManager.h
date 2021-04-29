#pragma once

#include "TcpClient.h"
#include <vector>
#include <list>
#include <string>
#include "parameter.h"
#include <fstream>
#include <stdint.h>
#include "Device.h"
#include "Axis.h"

class DeviceManager
{
public:
	enum ErrorID
	{
		NO_ERROR = 0,
		CONF_FILE_MISSING,
		CONF_FILE_PARSING
	};
	DeviceManager(const std::string& devDescFileName);
	const std::vector<Device>& getDevList() const;
	const std::vector<Axis>& getAxesList() const;
	uint32_t devCount() const;
	uint32_t axesCount() const;
	const std::string& getLogFile() const;
	void updateFirmware(const std::fstream& file);
	std::vector<uint8_t> getDevDescFileRaw();
	bool updateDevDescFile(const char* data, uint32_t size);
	std::list<uint8_t>& errors();
private:
	void parseDeviceDescriptionFile(std::fstream& file);
	std::string logFileStr;
	std::fstream devDescFile;
	std::string devDescFileStr;
	std::list<uint8_t> errorsList;
	bool devDescFileNotFound = false;
};
