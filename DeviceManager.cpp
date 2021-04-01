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
	queryArgs.reserve(16);
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

void DeviceManager::setAxisLimits(int devNum, double min, double max)
{
}

void DeviceManager::getAxisLimits(int devNum, double& min, double& max)
{
}

double DeviceManager::getAxisPos(int devNum, bool inUnits)
{
	string cmd = inUnits ? "UPOS" : "POS";
	sentCmdId = inUnits ? CmdQueryID::UPOS : CmdQueryID::POS;
	queryArgs.push_back(devNum);
	sendCmd("AXIS" + to_string(devNum) + ":STAT:"+ cmd +"?\n", true, 100);
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
	sendCmd("AXIS" + to_string(devNum) + ":SETZER\n", true, 100);
}

void DeviceManager::moveAllAxesToHome()
{
	for (size_t i = 0; i < devices.size(); i++)
		setAxisPosition(i, 0);
}

void DeviceManager::moveDevAxisToHome(int devNum)
{
	setAxisPosition(devNum, 0);
}

void DeviceManager::setAxisPosition(int devNum, double pos)
{
	sendCmd("AXIS" + to_string(devNum) + ":UMOV:ABS " + to_string(pos) +"\n", true, 100);
}

void DeviceManager::stopDevAxis(int devNum)
{
	sendCmd("AXIS" + to_string(devNum) + ":STOP\n", true, 100);
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
	string paramName = devices[devNum].getParameters()[paramId].name();
	sendCmd("DEV" + to_string(devNum) + ":SETP " + paramName + "," + to_string(value), true, 100);
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
	requestTimeout = period_ms * 100;
	while ((sentCmdId != CmdQueryID::Invalid) && requestTimeout)
	{
		requestTimeout--;
		usleep(10);
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
			for (int i = 0; i < devicesCount; i++)
			{
				const auto& dev = root["devices"][i];
				auto name = dev["name"].asString();
				auto type = dev["type"].asString();
				DeviceType devtype = (type == "mitsuservo_type_B") ? DeviceType::TypeB : DeviceType::TypeA;
				vector<Parameter> params;

				// Iterate parameter groups (from 'A' to ... symbol)
				for (char paramGroup[] = "PA"; !dev[paramGroup].isNull(); paramGroup[1]++)
				{
					int paramCount = dev[paramGroup].size();
					for (int j = 0; j < paramCount; j++)
					{
						const char** paramData = getDefaultParam(devtype, (ParamGroup)(paramGroup[1] - 'A'), j);
						string range = string(paramData[3]);
						string valueStr = dev[paramGroup][j]["value"].asString();

						// Divide range record "xxx-yyy" to 2 values: minimum (xxx) and maximum (yyy)
						// Note that if xxx < 0, then there is 2 hyphen signs and we must skip first
						size_t hyphenStartSearchPos = (paramData[3][0] == '-') ? 1 : 0;
						size_t hyphenPos = range.find('-', hyphenStartSearchPos);
						bool hex = strncmp(paramData[5], "HEX", 3) == 0;
						double min = 0, max = 0, value = 0;
						if (hex)
						{
							min = stoll(range.substr(0, hyphenPos), NULL, 16);
							max = stoll(range.substr(hyphenPos + 1), NULL, 16);
							//value = stoll(string(paramData[4]), &pos, 16);
							value = stoll(valueStr, NULL, 16);
						}
						else
						{
							min = stod(range.substr(0, hyphenPos));
							max = stod(range.substr(hyphenPos + 1));
							//value = stod(string(paramData[4]), &pos);
							value = stod(valueStr);
						}
						params.push_back({paramData[0], paramData[1], paramData[2], min, max, value, hex});
					}
				}
				devices.push_back({name, type, params});
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
	default:
		break;
	}
	sentCmdId = CmdQueryID::Invalid;
	queryArgs.clear();
}
