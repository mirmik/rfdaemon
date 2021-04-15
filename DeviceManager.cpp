#include <stdio.h>
#include <vector>
#include <filesystem>
#include "DeviceManager.h"
#include "jsoncpp/json/json.h"
#include "string_constants.h"
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

const std::vector<Axis>& DeviceManager::getAxesList() const
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

void DeviceManager::setAxisLimits(int axisNum, double min, double max)
{
	axes[axisNum].setMinLimit(min);
	axes[axisNum].setMaxLimit(max);
	sendCmd("AXIS" + to_string(axisNum) + ":SETTINGS:ULIMITS " + to_string(min) + ", " + to_string(max) +"\n");
}

void DeviceManager::getAxisLimits(int axisNum, double& min, double& max)
{
	queryArgs[0] = axisNum;
	sentCmdId = CmdQueryID::UBACKLIM;
	sendCmd("AXIS" + to_string(axisNum) + ":SETTINGS:UBACKLIM?\n");
	waitAnswer();
	sentCmdId = CmdQueryID::UFORWLIM;
	sendCmd("AXIS" + to_string(axisNum) + ":SETTINGS:UFORWLIM?\n");
	waitAnswer();
	min = axes[axisNum].minLimit();
	max = axes[axisNum].maxLimit();
}

void DeviceManager::getActualAxesNum()
{
	sentCmdId = CmdQueryID::AXESTOT;
	sendCmd("SYST:AXESTOT?\n");
	waitAnswer();
}

void DeviceManager::getActualDevsNum()
{
	sentCmdId = CmdQueryID::DEVSTOT;
	sendCmd("SYST:DEVSTOT?\n");
	waitAnswer();
}

double DeviceManager::getAxisPos(int axisNum, bool inUnits)
{
	string cmd = inUnits ? "UPOS" : "POS";
	sentCmdId = inUnits ? CmdQueryID::UPOS : CmdQueryID::POS;
	queryArgs[0] = axisNum;
	sendCmd("AXIS" + to_string(axisNum) + ":STAT:"+ cmd +"?\n");
	if (waitAnswer())
		axes[axisNum].pos = axes[axisNum].prevPos;
	else
		axes[axisNum].prevPos = axes[axisNum].pos;
	return axes[axisNum].pos;
}

void DeviceManager::getSensorData(int devNum)
{
	// Quering shaft torque value if device is servoamplifier with connected motor
	if (devices[devNum].type() == Device::Type::ServoTypeA &&
		devices[devNum].type() == Device::Type::ServoTypeB)
	{
		//sentCmdId = CmdQueryID::TORQUE;
		//queryArgs[0] = devNum;
		//sendCmd("DEV" + to_string(devNum) + ":TORQUE?\n");
		//waitAnswer();
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

const vector<Parameter>& DeviceManager::getParameterList(int devNum)
{
	return devices[devNum].parameters();
}

const double DeviceManager::getParameterValue(int devNum, int paramId)
{
	return devices[devNum].parameters()[paramId].getValue();
}

void DeviceManager::setParameterValue(int devNum, uint16_t paramId, double value)
{
	devices[devNum].setParameterValue(paramId, value);
	string paramName = devices[devNum].parameters()[paramId].name();
	sendCmd("DEV" + to_string(devNum) + ":SETP " + paramName + "," + to_string(value));
	printf("Param %s of dev %d set to %.3f\n", paramName.c_str(), devNum, (float)value);
}

void DeviceManager::setParameterValues(int devNum, const vector<uint16_t>& idList, const vector<double>& list)
{
	for (size_t i = 0; i < idList.size(); i++)
		setParameterValue(devNum, idList[i], list[i]);
}

void DeviceManager::setParameterValues(int devNum, const vector<double>& list)
{
	size_t minParamCount = min(list.size(), devices[devNum].parameters().size());
	for (size_t i = 0; i < minParamCount; i++)
		setParameterValue(devNum, i, list[i]);
}

void DeviceManager::updateFirmware(const fstream& file)
{

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
			vector<Parameter> params;

			for (int i = 0; i < devicesCount; i++)
			{
				params.clear();
				const auto& dev = root["devices"][i];
				name = dev["name"].asString();
				type = dev["type"].asString();
				
				if (type.find("servo") != string::npos)
					devtype = (type.find("type_B") != string::npos) ? Device::Type::ServoTypeB : Device::Type::ServoTypeA;
				else if (type.find("sync") != string::npos)
					devtype = Device::Type::Sync;

				// Iterate parameter groups (from 'A' to ... symbol)
				if (devtype == Device::Type::ServoTypeA || devtype == Device::Type::ServoTypeB)
				{
					for (char paramGroup[] = "PA"; !dev[paramGroup].isNull(); paramGroup[1]++)
					{
						int paramCount = dev[paramGroup].size();
						for (int j = 0; j < paramCount; j++)
						{
							const char** paramDataDefault = getDefaultParam(devtype, (ParamGroup)(paramGroup[1] - 'A'), j);
							range = string(paramDataDefault[3]);
							valueStr = dev[paramGroup][j]["value"].asString();

							// Divide range record "xxx-yyy" to 2 values: minimum (xxx) and maximum (yyy)
							// Note that if xxx < 0, then there is 2 hyphen signs and we must skip first
							size_t hyphenStartSearchPos = (paramDataDefault[3][0] == '-') ? 1 : 0;
							size_t hyphenPos = range.find('-', hyphenStartSearchPos);
							Parameter::Type type = Parameter::Type::Int;
							double min = 0, max = 0, value = 0;
							minStr = range.substr(0, hyphenPos);
							maxStr = range.substr(hyphenPos + 1);
							const char* fppos = strchr(paramDataDefault[3] + hyphenPos, '.');

							if (fppos)
							{
								int precision = strlen(fppos) - 2;
								type = (Parameter::Type)(Parameter::Type::FracPoint1 + precision);
								min = stod(minStr);
								max = stod(maxStr);
								value = stod(valueStr); //value = stod(string(paramData[4]), &pos);
							}
							else
							{
								int base = 10;
								if (!strncmp(paramDataDefault[5], "HEX", 3))
								{
									base = 16;
									type = Parameter::Type::Hex;
								}

								min = stoll(minStr, NULL, base);
								max = stoll(maxStr, NULL, base);
								value = stoll(valueStr, NULL, base); //value = stoll(string(paramData[4]), &pos, base);
							}
							params.push_back({ paramDataDefault[0], paramDataDefault[1], paramDataDefault[2], value, min, max, type });
						}
					}
				}
				devices.push_back({name, devtype, params});
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
			}
		}
	}
}

void DeviceManager::parseReceivedData(const vector<uint8_t>& data)
{
	double answer = strtod((char*)data.data(), NULL);
	switch (sentCmdId)
	{
	case DeviceManager::Invalid:
		break;
	case DeviceManager::POS:
	case DeviceManager::UPOS:
		axes[queryArgs[0]].pos = answer;
		break;
	case DeviceManager::UFORWLIM:
		axes[queryArgs[0]].setMinLimit(answer);
		break;
	case DeviceManager::UBACKLIM:
		axes[queryArgs[0]].setMaxLimit(answer);
		break;
	case DeviceManager::AXESTOT:
		break;
	case DeviceManager::DEVSTOT:
		break;
	case DeviceManager::TORQUE:
		devices[queryArgs[0]].updateSensorValue(answer);
		break;
	default:
		break;
	}
	sentCmdId = CmdQueryID::Invalid;
}
