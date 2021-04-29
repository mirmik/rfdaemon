#include <stdio.h>
#include <vector>
#include <filesystem>
#include <limits>
#include <cmath>
#include "DeviceManager.h"
#include "jsoncpp/json/json.h"
#include "config_json_default.h"

using namespace std;

DeviceManager::DeviceManager(const string& devDescFileName)
{
	memset(queryArgs, 0, sizeof(queryArgs));
	devDescFileStr = devDescFileName;
	if (filesystem::exists(devDescFileName))
	{
		devDescFile = fstream(devDescFileName, fstream::in);
		printf("Device description file found.\n");
	}
	else
	{
		errorsList.push_back(ErrorID::CONF_FILE_MISSING);
		devDescFileNotFound = true;
		devDescFile = fstream(devDescFileName, fstream::in | fstream::out | fstream::trunc);
		devDescFile.seekp(0);
		devDescFile.write(defaultDevDescFile, defaultDevDescFileLen);
		devDescFile.flush();
		printf("Device description file not found, create default.\n");
	}
	parseDeviceDescriptionFile(devDescFile);
}

const vector<Device>& DeviceManager::getDevList() const
{
	return devices;
}

const vector<Axis>& DeviceManager::getAxesList() const
{
	return axes;
}

uint32_t DeviceManager::devCount() const
{
	return devices.size();
}

uint32_t DeviceManager::axesCount() const
{
	return axes.size();
}

const string& DeviceManager::getLogFile() const
{
	return logFileStr;
}

void DeviceManager::writeAxisLimits(int axisNum, double min, double max)
{
	axes[axisNum].setMinLimit(min);
	axes[axisNum].setMaxLimit(max);
	sendCmd("AXIS" + to_string(axisNum) + ":SETTINGS:ULIMITS " + to_string(min) + ", " + to_string(max) +"\n");
}

void DeviceManager::readAxisLimits(int axisNum, double& min, double& max)
{
	queryArgs[0] = axisNum;
	sentCmdId = CmdQueryID::UBACKLIM;
	sendCmd("AXIS" + to_string(axisNum) + ":SETTINGS:UBACKLIM?\n");
	lastCmdTimeout = waitAnswer();
	sentCmdId = CmdQueryID::UFORWLIM;
	sendCmd("AXIS" + to_string(axisNum) + ":SETTINGS:UFORWLIM?\n");
	lastCmdTimeout = waitAnswer();
	min = axes[axisNum].minLimit();
	max = axes[axisNum].maxLimit();
}

size_t DeviceManager::readActualAxesNum()
{
	sentCmdId = CmdQueryID::AXESTOT;
	sendCmd("SYST:AXESTOT?\n");
	lastCmdTimeout = waitAnswer();
	return axes.size();
}

size_t DeviceManager::readActualDevsNum()
{
	sentCmdId = CmdQueryID::DEVSTOT;
	sendCmd("SYST:DEVSTOT?\n");
	lastCmdTimeout = waitAnswer();
	return devices.size();
}

double DeviceManager::readAxisPos(int axisNum, bool inUnits)
{
	string cmd = inUnits ? "UPOS" : "POS";
	sentCmdId = inUnits ? CmdQueryID::UPOS : CmdQueryID::POS;
	queryArgs[0] = axisNum;
	sendCmd("AXIS" + to_string(axisNum) + ":STAT:"+ cmd +"?\n");
	if (waitAnswer())
	{
		axes[axisNum].pos = axes[axisNum].prevPos;
		lastCmdTimeout = true;
	}
	else
	{
		axes[axisNum].prevPos = axes[axisNum].pos;
		lastCmdTimeout = false;
	}
	return axes[axisNum].pos;
}

void DeviceManager::readSensorData(int devNum)
{
	// Quering shaft torque value if device is servoamplifier with connected motor
	Device::Type type = devices[devNum].type();
	if (type != Device::Type::Sync && type != Device::Type::SimulatorSync)
	{
		//sentCmdId = CmdQueryID::TORQUE;
		//queryArgs[0] = devNum;
		//sendCmd("DEV" + to_string(devNum) + ":TORQUE?\n");
		//lastCmdTimeout = waitAnswer();
		devices[devNum].updateSensorValue((rand() % 10000) / 10000.0 + devNum);
	}
}

void DeviceManager::setAllAxesToZero()
{
	for (size_t i = 0; i < axes.size(); i++)
		setAxisToZero(i);
}

void DeviceManager::setAxisToZero(int axisNum)
{
	sendCmd("AXIS" + to_string(axisNum) + ":SETZER\n");
}

void DeviceManager::moveAllAxesToHome()
{
	for (size_t i = 0; i < axes.size(); i++)
		setAxisAbsPosition(i, 0);
}

void DeviceManager::moveAxisToHome(int axisNum)
{
	setAxisAbsPosition(axisNum, 0);
}

void DeviceManager::setAxisAbsPosition(int axisNum, double pos)
{
	sendCmd("AXIS" + to_string(axisNum) + ":UMOV:ABS " + to_string(pos) +"\n");
}

void DeviceManager::stopAxis(int axisNum)
{
	sendCmd("AXIS" + to_string(axisNum) + ":STOP\n");
}

void DeviceManager::stopAllAxes()
{
	sendCmd("SYST:STOP\n");
}

void DeviceManager::jogAxis(int devNum, double offset)
{
	printf("\"JOG\" command not implemented.\n");
}

double DeviceManager::readParameterValue(int devNum, string name, bool& success)
{
	((int*)&queryArgs)[0] = devNum;
	sentCmdId = CmdQueryID::PARAM;
	sendCmd("DEV" + to_string(devNum) + ":PARAM? \"" + name + "\"");
	lastCmdTimeout = waitAnswer();
	double value = *(double*)answerBuffer;
	printf("Value of param %s of dev %d is %.3f\n", name.c_str(), devNum, (float)value);
	success = lastCmdTimeout || lastCmdErr;
	return value;
}

bool DeviceManager::writeParameterValue(int devNum, string name, double value)
{
	sendCmd("DEV" + to_string(devNum) + ":PARAM \"" + name + "\"," + to_string(value));
	printf("Param %s of dev %d set to %.3f\n", name.c_str(), devNum, (float)value);
	return true;
}

size_t DeviceManager::writeParameterValues(int devNum, const vector<string>& names, const vector<double>& values)
{
	int paramsWritten = 0;
	for (size_t i = 0; i < names.size(); i++)
	{
		if (writeParameterValue(devNum, names[i], values[i]))
			paramsWritten++;
	}
	return paramsWritten;
}

void DeviceManager::updateFirmware(const fstream& file)
{

}

vector<uint8_t> DeviceManager::getDevDescFileRaw()
{
	vector<uint8_t> data;
	devDescFile.open(devDescFileStr, fstream::in);
	if (devDescFile.is_open())
	{
		stringstream buffer;
		buffer << devDescFile.rdbuf();
		string s = buffer.str();
		data.assign(s.begin(), s.end());
		devDescFile.close();
	}
	return data;
}

bool DeviceManager::updateDevDescFile(const char* data, uint32_t size)
{
	bool error = true;
	devDescFile.open(devDescFileStr, fstream::out | fstream::trunc);
	if (devDescFile.is_open())
	{
		devDescFile.seekp(0);
		devDescFile.write(data, size);
		error = (devDescFile.rdstate() & (ios::failbit | ios::badbit)) != 0;
		devDescFile.flush();
		parseDeviceDescriptionFile(devDescFile);
		devDescFile.close();
	}
	return error;
}

list<uint8_t>& DeviceManager::errors()
{
	return errorsList;
}

// Return true if timeout occurred
bool DeviceManager::waitAnswer(unsigned long period_ms)
{
	requestTimeout = period_ms * 200;
	while ((sentCmdId != CmdQueryID::Invalid) && requestTimeout)
	{
		requestTimeout--;
		usleep(5);
	}
	if (requestTimeout == 0)
		printf("Timeout while waiting for answer from server at port %d.\n", port());
	return requestTimeout == 0;
}

void DeviceManager::parseDeviceDescriptionFile(std::fstream& file)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;

	if (file.is_open())
	{
		if (Json::parseFromStream(builder, file, &root, &errs))
		{
			int devicesCount = root["devices"].size();
			if (!devices.empty())
				devices.clear();

			string name, type, range, valueStr, minStr, maxStr;
			Device::Type devtype = Device::Type::Unknown;

			for (int i = 0; i < devicesCount; i++)
			{
				const auto& dev = root["devices"][i];
				name = dev["name"].asString();
				type = dev["type"].asString();

				if (type == "mitsuservo_type_A" || type == "mitsuservo")
					devtype = Device::ServoTypeA;
				else if (type == "mitsuservo_type_B")
					devtype = Device::ServoTypeB;
				else if (type == "sync")
					devtype = Device::Sync;
				else if (type == "simulator_servo")
					devtype = Device::Simulator;
				else if (type == "simulator_syncro")
					devtype = Device::SimulatorSync;
				else
					devtype = Device::Unknown;
				devices.push_back({name, devtype});
			}

			int axesCount = root["axes"].size();
			if (!axes.empty())
				axes.clear();

			for (int i = 0; i < axesCount; i++)
			{
				auto& axRecord = root["axes"][i];

				Axis::Type type = (axRecord["type"].asString() == "simple") ?
					Axis::Type::Simple : Axis::Type::Sync;
				Axis::Motion motion = (axRecord["motionType"].asString() == "round") ?
					Axis::Motion::Round : Axis::Motion::Linear;
				Axis axis = Axis(axRecord["name"].asString(), type, motion);
				string syncDev = "";
				string name = axRecord["servo"].asString();

				if (type == Axis::Type::Sync)
					syncDev = axRecord["syncro"].asString();

				for (auto& dev : devices)
				{
					if (dev.name() == name)
						axis.linkDevice(&dev);
					if (type == Axis::Type::Sync)
						if (dev.name() == syncDev)
							axis.setSyncDevice(&dev);
				}
				axes.push_back(axis);
				if (axes[i].linkedDevs()[0])
					axes[i].linkedDevs()[0]->setAxis(&axes[i]);
			}
		}
		else
			errorsList.push_back(ErrorID::CONF_FILE_PARSING);
		file.close();
	}
}

void DeviceManager::parseReceivedData(const vector<uint8_t>& data)
{
	double value = extractValue(data);
	bool convSuccess = !isnan(value);
	ErrorID err = NO_ERROR;

	switch (sentCmdId)
	{
	case DeviceManager::Invalid:
		break;
	case DeviceManager::POS:
	case DeviceManager::UPOS:
		if (convSuccess)
			axes[queryArgs[0]].pos = value;
		else
			err = WRONG_ANSWER;
		break;
	case DeviceManager::UFORWLIM:
		if (convSuccess)
			axes[queryArgs[0]].setMinLimit(value);
		else
			err = WRONG_ANSWER;
		break;
	case DeviceManager::UBACKLIM:
		if (convSuccess)
			axes[queryArgs[0]].setMaxLimit(value);
		else
			err = WRONG_ANSWER;
		break;
	case DeviceManager::AXESTOT:
	case DeviceManager::DEVSTOT:
		break;
	case DeviceManager::TORQUE:
		if (convSuccess)
			devices[queryArgs[0]].updateSensorValue(value);
		else
			err = WRONG_ANSWER;
		break;
	case DeviceManager::PARAM:
		if (convSuccess)
			*(double*)answerBuffer = value;
		else
			err = WRONG_ANSWER;
		break;
	default:
		break;
	}
	lastCmdErr = err;
	if (err)
		errorsList.push_back(err);
	sentCmdId = CmdQueryID::Invalid;
}

double DeviceManager::extractValue(const std::vector<uint8_t>& data)
{
	char* end;
	double answer = strtod((char*)data.data(), &end);
	if (end == (char*)data.data())
		return numeric_limits<double>::quiet_NaN();
	else
		return answer;
}
