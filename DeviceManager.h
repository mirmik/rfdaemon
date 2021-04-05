#pragma once

#include "TcpClient.h"
#include <vector>
#include <string>
#include "parameter.h"
#include <fstream>
#include <stdint.h>
#include "Device.h"

class DeviceManager : public TcpClient
{
public:
	enum CmdQueryID
	{
		Invalid = 0,
		POS,
		UPOS
	};
	DeviceManager(const std::string& devDescFileName);
	const std::vector<std::string> getDevList() const;
	uint32_t getDevCount() const;
	const std::string& getLogFile() const;
	double getMeasuredValue(int devNum);
	void setAxisLimits(int devNum, double min, double max);
	void getAxisLimits(int devNum, double& min, double& max);
	double getAxisPos(int devNum, bool inUnits = true);
	void setAllAxesToZero();
	void setDevAxisToZero(int devNum);
	void moveAllAxesToHome();
	void moveDevAxisToHome(int devNum);
	void setAxisPosition(int devNum, double pos);
	void stopDevAxis(int devNum);
	void stopAllAxes();
	void jogAxis(int devNum, double offset);
	const std::vector<Parameter>& getParameterList(int devNum);
	const double getParameterValue(int devNum, int paramId);
	void setParameterValue(int devNum, uint16_t paramId, double value);
	void setParameterValues(int devNum, const std::vector<uint16_t>& idList, const std::vector<double>& list);
	void setParameterValues(int devNum, const std::vector<double>& list);
	void updateFirmware(const std::fstream& file);
private:
	// Return true if timeout occurred
	bool waitAnswer(unsigned long period_ms = 500);
	void parseDeviceDescriptionFile(std::fstream& file);
	virtual void parseReceivedData(const std::vector<uint8_t>& data) override;
	std::vector<Device> devices;
	std::string logFileStr;
	std::vector<double> measuredValues;
	std::fstream devDescFile;
	std::string devDescFileStr;
	CmdQueryID sentCmdId = CmdQueryID::Invalid;
	std::vector<uint8_t> queryArgs;
	bool devDescFileNotFound = false;
};
