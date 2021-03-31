#include <stdio.h>
#include <vector>
#include <filesystem>
#include "DeviceManager.h"
#include "jsoncpp/json/json.h"
#include "string_constants.h"
#include "config_json_default.h"

using namespace std;

DeviceManager::DeviceManager(uint16_t port, const string& devDescFileName) :
	TcpClient(port)
{
	devDescFileStr = devDescFileName;
	if (filesystem::exists(devDescFileName))
		devDescFile = fstream(devDescFileName, fstream::in);
	else
	{
		devDescFileNotFound = true;
		devDescFile = fstream(devDescFileName, fstream::in | fstream::out | fstream::trunc);
		devDescFile.seekp(0);
		devDescFile.write(defaultDevDescFile, defaultDevDescFileLen);
		devDescFile.flush();
		devDescFile.close();
		printf("%d\n", strlen(defaultDevDescFile));
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

float DeviceManager::getMeasuredValue(int devNum)
{
	return measuredValues[devNum];
}

void DeviceManager::setAxisLimits(int devNum, double min, double max)
{
}

void DeviceManager::getAxisLimits(int devNum, double& min, double& max)
{
}

void DeviceManager::setAllAxesToZero()
{
	for (size_t i = 0; i < devices.size(); i++)
		setDevAxisToZero(i);
}

void DeviceManager::setDevAxisToZero(int devNum)
{
	sendCmd("AXIS" + to_string(devNum) + ":SETZER\r\n", true, 100);
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
	sendCmd("AXIS" + to_string(devNum) + ":UMOV:ABS " + to_string((double)pos) +"\r\n", true, 100);
}

void DeviceManager::stopDevAxis(int devNum)
{
	sendCmd("AXIS" + to_string(devNum) + ":STOP\r\n", true, 100);
}

void DeviceManager::jogAxis(int devNum, double offset)
{
	printf("\"JOG\" command not implemented.\n");
}

const vector<Parameter>& DeviceManager::getParameterList(int devNum)
{
	static vector<Parameter> p;
	return p;
}

const double DeviceManager::getParameterValue(int devNum, int paramId)
{
	return double();
}

// PA:32 PB:64 PC:80 PD:48 PE:64 PF:48 PL:48, 384 total
void DeviceManager::setParameterValue(int devNum, uint16_t paramId, double value)
{
	string paramName = devices[devNum].getParameters()[paramId].name();
	sendCmd("DEV" + to_string(devNum) + ":SETParam " + paramName + "," + to_string(value), true, 100);
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

void DeviceManager::parseDeviceDescriptionFile(std::fstream& file)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;

	if (file.is_open())
	{
		if (parseFromStream(builder, file, &root, &errs))
		{
			int devicesCount = root["devices"].size();
			for (int i = 0; i < devicesCount; i++)
			{
				auto name = root["devices"][i]["name"].asString();
				auto type = root["devices"][i]["type"].asString();
				DeviceType devtype = (type == "mitsuservo_type_B") ? DeviceType::TypeB : DeviceType::TypeA;
				vector<Parameter> params;

				// Iterate parameter groups (from 'A' to ... symbol)
				for (char paramGroup[] = "PA"; !root["devices"][i][paramGroup].isNull(); paramGroup[1]++)
				{
					int paramCount = root["devices"][i][paramGroup].size();
					for (int j = 0; j < paramCount; j++)
					{
						const char** paramData = getDefaultParam(devtype, (ParamGroup)(paramGroup[1] - 'A'), j);
						string range = string(paramData[3]);

						// Divide range record "xxx-yyy" to 2 values: minimum (xxx) and maximum (yyy)
						// Note that if xxx < 0, then there is 2 hyphen signs and we mustt skip first
						size_t hyphenPos = (paramData[3][0] == '-') ? range.find('-', 1) : range.find('-');
						bool hex = strncmp(paramData[5], "HEX", 3) == 0;
						double min = 0;
						double max = 0;
						double value = 0;
						size_t pos;
						if (hex)
						{
							min = stoll(range.substr(0, hyphenPos), &pos, 16);
							max = stoll(range.substr(hyphenPos + 1), &pos, 16);
							value = stoll(string(paramData[4]), &pos, 16);
						}
						else
						{
							min = stod(range.substr(0, hyphenPos), &pos);
							max = stod(range.substr(hyphenPos + 1), &pos);
							value = stod(string(paramData[4]), &pos);
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
	case 0:
		break;
	default:
		break;
	}
}
