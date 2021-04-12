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

const vector<string> DeviceManager::getDevList() const
{
	vector<string> devNames;
	for (auto& d : devices)
		devNames.push_back(d.getName());
	return devNames;
}

uint32_t DeviceManager::getDevCount() const
{
	return devices.size();
}

const string& DeviceManager::getLogFile() const
{
	return logFileStr;
}

double DeviceManager::getMeasuredValue(int devNum)
{
	return measuredValues[devNum];
}

void DeviceManager::setMeasuredValue(int devNum, double val)
{
	measuredValues[devNum] = val;
}

void DeviceManager::setAxisLimits(int devNum, double min, double max)
{
	devices[devNum].axisLimitMin = min;
	devices[devNum].axisLimitMax = max;
	sendCmd("AXIS" + to_string(devNum) + ":SETTINGS:ULIMITS " + to_string(min) + ", " + to_string(max) +"\n");
}

void DeviceManager::getAxisLimits(int devNum, double& min, double& max)
{
	queryArgs[0] = devNum;
	sentCmdId = CmdQueryID::UBACKLIM;
	sendCmd("AXIS" + to_string(devNum) + ":SETTINGS:UBACKLIM?\n");
	waitAnswer();
	sentCmdId = CmdQueryID::UFORWLIM;
	sendCmd("AXIS" + to_string(devNum) + ":SETTINGS:UFORWLIM?\n");
	waitAnswer();
	min = devices[devNum].axisLimitMin;
	max = devices[devNum].axisLimitMax;
}

double DeviceManager::getAxisPos(int devNum, bool inUnits)
{
	string cmd = inUnits ? "UPOS" : "POS";
	sentCmdId = inUnits ? CmdQueryID::UPOS : CmdQueryID::POS;
	queryArgs[0] = devNum;
	sendCmd("AXIS" + to_string(devNum) + ":STAT:"+ cmd +"?\n");
	if (waitAnswer())
		devices[devNum].axisPos = devices[devNum].prevAxisPos;
	else
		devices[devNum].prevAxisPos = devices[devNum].axisPos;
	return devices[devNum].axisPos;
}

void DeviceManager::setAllAxesToZero()
{
	for (size_t i = 0; i < devices.size(); i++)
		setDevAxisToZero(i);
}

void DeviceManager::setDevAxisToZero(int devNum)
{
	sendCmd("AXIS" + to_string(devNum) + ":SETZER\n");
}

void DeviceManager::moveAllAxesToHome()
{
	for (size_t i = 0; i < devices.size(); i++)
		setAxisAbsPosition(i, 0);
}

void DeviceManager::moveDevAxisToHome(int devNum)
{
	setAxisAbsPosition(devNum, 0);
}

void DeviceManager::setAxisAbsPosition(int devNum, double pos)
{
	sendCmd("AXIS" + to_string(devNum) + ":UMOV:ABS " + to_string(pos) +"\n");
}

void DeviceManager::stopDevAxis(int devNum)
{
	sendCmd("AXIS" + to_string(devNum) + ":STOP\n");
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
	return devices[devNum].getParameters();
}

const double DeviceManager::getParameterValue(int devNum, int paramId)
{
	return devices[devNum].getParameters()[paramId].getValue();
}

void DeviceManager::setParameterValue(int devNum, uint16_t paramId, double value)
{
	devices[devNum].setParameterValue(paramId, value);
	string paramName = devices[devNum].getParameters()[paramId].name();
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
	size_t minParamCount = min(list.size(), devices[devNum].getParameters().size());
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
			measuredValues.resize(devicesCount);
			string name, type, range, valueStr, minStr, maxStr;
			DeviceType devtype = DeviceType::None;
			vector<Parameter> params;

			for (int i = 0; i < devicesCount; i++)
			{
				params.clear();
				const auto& dev = root["devices"][i];
				name = dev["name"].asString();
				type = dev["type"].asString();
				
				if (type.find("mitsuservo") != string::npos)
					devtype = (type.find("type_B") != string::npos) ? DeviceType::TypeB : DeviceType::TypeA;
				else
					devtype = DeviceType::None;

				// Iterate parameter groups (from 'A' to ... symbol)
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
						params.push_back({paramDataDefault[0], paramDataDefault[1], paramDataDefault[2], value, min, max, type});
					}
				}
				devices.push_back({name, type, params});
				measuredValues[i] = 0;
			}
		}
	}
}

void DeviceManager::parseReceivedData(const vector<uint8_t>& data)
{
	switch (sentCmdId)
	{
	case DeviceManager::Invalid:
		break;
	case DeviceManager::POS:
	case DeviceManager::UPOS:
		devices[queryArgs[0]].axisPos = strtod((char*)data.data(), NULL);
		break;
	case DeviceManager::UFORWLIM:
		devices[queryArgs[0]].axisLimitMax = strtod((char*)data.data(), NULL);
		break;
	case DeviceManager::UBACKLIM:
		devices[queryArgs[0]].axisLimitMin = strtod((char*)data.data(), NULL);
		break;
	default:
		break;
	}
	sentCmdId = CmdQueryID::Invalid;
}
