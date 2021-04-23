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
	static constexpr int ReadAllParamsMaxRetries = 5;
	enum CmdQueryID
	{
		Invalid = 0,
		POS,
		UPOS,
		UFORWLIM,
		UBACKLIM,
		AXESTOT,
		DEVSTOT,
		TORQUE,
		PARAM
	};
	enum ErrorID
	{
		NO_ERROR = 0,
		CONF_FILE_MISSING,
		CONF_FILE_PARSING,
		WRONG_ANSWER
	};
	DeviceManager(const std::string& devDescFileName);
	const std::vector<Device>& getDevList() const;
	const std::vector<Axis>& getAxesList() const;
	uint32_t devCount() const;
	uint32_t axesCount() const;
	const std::string& getLogFile() const;
	void writeAxisLimits(int axisNum, double min, double max);
	void readAxisLimits(int axisNum, double& min, double& max);
	size_t readActualAxesNum();
	size_t readActualDevsNum();
	double readAxisPos(int axisNum, bool inUnits = true);
	void readSensorData(int devNum);
	void setAllAxesToZero();
	void setAxisToZero(int axisNum);
	void moveAllAxesToHome();
	void moveAxisToHome(int axisNum);
	void setAxisAbsPosition(int axisNum, double pos);
	void stopAxis(int axisNum);
	void stopAllAxes();
	void jogAxis(int devNum, double offset);
	double readParameterValue(int devNum, std::string name, bool& success);
	bool writeParameterValue(int devNum, std::string name, double value);
	size_t writeParameterValues(int devNum, const std::vector<std::string>& names, const std::vector<double>& values);
	void updateFirmware(const std::fstream& file);
	std::vector<uint8_t> getDevDescFileRaw();
	bool updateDevDescFile(const char* data, uint32_t size);
private:
	// Return true if timeout occurred
	bool waitAnswer(unsigned long period_ms = 100UL);
	void parseDeviceDescriptionFile(std::fstream& file);
	virtual void parseReceivedData(const std::vector<uint8_t>& data) override;
	double extractValue(const std::vector<uint8_t>& data);
	std::vector<Device> devices;
	std::vector<Axis> axes;
	std::string logFileStr;
	std::fstream devDescFile;
	std::string devDescFileStr;
	CmdQueryID sentCmdId = CmdQueryID::Invalid;
	char queryArgs[65535];
	char answerBuffer[65535];
	bool devDescFileNotFound = false;
	std::vector<ErrorID> errors;
	bool lastCmdTimeout = false;
	ErrorID lastCmdErr = NO_ERROR;
};
