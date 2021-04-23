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
		Simulator,
		SimulatorSync,
		Unknown
	};
	enum EncoderType
	{
		Internal = 0,
		External
	};
	Device(const std::string& name, Type type);
	const std::string& name() const;
	Axis* axis() const;
	Type type() const;
	double sensorValue() const;

protected:
	void setAxis(Axis* axis);
	void updateSensorValue(double value);
private:
	double _sensorValue = 0;
	Axis* _axis = 0;
	std::string _name;
	Type _type;
};
