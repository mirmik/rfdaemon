#pragma once

#include <vector>
#include <string>
#include "parameter.h"

class DeviceManager;
class Axis;

class Device
{
	friend class DeviceManager;
public:
	enum Type
	{
		ServoTypeA,
		ServoTypeB,
		Sync,
		Unknown
	};
	Device(const std::string& name, Type type, const std::vector<Parameter>& parameters);
	const std::vector<Parameter>& parameters() const;
	const std::string& name() const;
	Axis* axis() const;
	Type type() const;
	double sensorValue() const;

protected:
	void setParameterValue(uint16_t id, double value);
	void setAxis(Axis* axis);
	void updateSensorValue(double value);
private:
	double _sensorValue = 0;
	Axis* _axis = 0;
	std::string _name;
	Type _type;
	std::vector<Parameter> _parameters;
};
