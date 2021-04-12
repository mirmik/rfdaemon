#pragma once

#include <vector>
#include <string>
#include "parameter.h"

class DeviceManager;

class Device
{
	friend class DeviceManager;
public:
	Device(const std::string& name, const std::string& type, const std::vector<Parameter>& parameters);
	const std::vector<Parameter>& getParameters() const;
	void setParameterValue(uint16_t id, double value);
	const std::string& getName() const;
	double axisPosition();
protected:
	double axisPos = 0;
	double prevAxisPos = 0;
	double axisLimitMin = 0;
	double axisLimitMax = 0;
private:
	std::string _name;
	std::string _type;
	std::vector<Parameter> _parameters;
};
