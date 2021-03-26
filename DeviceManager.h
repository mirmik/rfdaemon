#pragma once

#include "TcpClient.h"
#include <vector>
#include <string>
#include "parameter.h"

class DeviceManager : public TcpClient
{
public:
	DeviceManager(uint16_t port);
	const std::vector<std::string>& getDevList() const;
	const std::string& getLogFile() const;
	float getMeasuredValue(int devNum);
	void setAxisLimits(int devNum, double min, double max);
	void getAxisLimits(int devNum, double& min, double& max);
	void setAllAxesToZero();
	void moveAllAxesToHome();
	const std::vector<Parameter>& getParameterList(int devNum);
	void setParameterList(int devNum, const std::vector<Parameter>& list);
	void updateFirmware(const fstream& file);
private:
	std::vector<std::string> devices;
	std::string logFileStr;
	std::vector<float> measuredValues;
};
