#pragma once

#include "TcpClient.h"
#include <vector>
#include <string>
#include "parameter.h"
#include <fstream>
#include <stdint.h>
#include "Device.h"
#include "Axis.h"

class DeviceManager : public TcpClient
{
public:
	enum CmdQueryID
	{
		Invalid = 0,
		POS,
		UPOS,
		UFORWLIM,
		UBACKLIM,
		AXESTOT,
		DEVSTOT,
		TORQUE
	};
	DeviceManager(const std::string& devDescFileName);
	const std::vector<Device>& getDevList() const;
	const std::vector<Axis>& getAxesList() const;
	uint32_t devCount() const;
	uint32_t axesCount() const;
	const std::string& getLogFile() const;
	void setAxisLimits(int axisNum, double min, double max);
	void getAxisLimits(int axisNum, double& min, double& max);
	void getActualAxesNum();
	void getActualDevsNum();
	double getAxisPos(int axisNum, bool inUnits = true);
	void getSensorData(int devNum);
	void setAllAxesToZero();
	void setAxisToZero(int axisNum);
	void moveAllAxesToHome();
	void moveAxisToHome(int axisNum);
	void setAxisAbsPosition(int axisNum, double pos);
	void stopAxis(int axisNum);
	void stopAllAxes();
	void jogAxis(int devNum, double offset);
	const std::vector<Parameter>& getParameterList(int devNum);
	const double getParameterValue(int devNum, int paramId);
	void setParameterValue(int devNum, uint16_t paramId, double value);
	void setParameterValues(int devNum, const std::vector<uint16_t>& idList, const std::vector<double>& list);
	void setParameterValues(int devNum, const std::vector<double>& list);
	void updateFirmware(const std::fstream& file);
	std::vector<uint8_t> getDevDescFileRaw();
	bool updateDevDescFile(const char* data, uint32_t size);
private:
	// Return true if timeout occurred
	bool waitAnswer(unsigned long period_ms = 100UL);
	void parseDeviceDescriptionFile(std::fstream& file);
	virtual void parseReceivedData(const std::vector<uint8_t>& data) override;
	std::vector<Device> devices;
	std::vector<Axis> axes;
	std::string logFileStr;
	std::fstream devDescFile;
	std::string devDescFileStr;
	CmdQueryID sentCmdId = CmdQueryID::Invalid;
	char queryArgs[65535];
	bool devDescFileNotFound = false;
};
