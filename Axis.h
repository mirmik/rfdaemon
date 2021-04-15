#pragma once

#include <string>
#include <vector>

class Device;
class DeviceManager;

class Axis
{
	friend class DeviceManager;
public:
	enum Type : unsigned char
	{
		Simple = 0,
		Sync
	};
	enum Motion : unsigned char
	{
		Linear = 0,
		Round
	};
	Axis(const std::string& name, Type type, Motion motionType);
	const std::string& name() const;
	Device* syncDev() const;
	Type type() const;
	Motion motionType() const;
	double position() const;
	double minLimit() const;
	double maxLimit() const;
	const std::vector<Device*>& linkedDevs() const;
protected:
	void linkDevice(Device* dev);
	void linkDevices(const std::vector<Device*>& devs);
	void setSyncDevice(Device* dev);
	void setMinLimit(double value);
	void setMaxLimit(double value);
	void setPosition(double value);
	double pos = 0;
	double prevPos = 0;
private:
	double _minLimit = 0;
	double _maxLimit = 0;
	std::string _name;
	std::vector<Device*> linkedDevices;
	Device* syncDevice = NULL;
	Type _type = Type::Simple;
	Motion _motionType = Motion::Linear;
	//std::string _servo;
	//std::string _sync;
	//double maxRpm = 0;
	//double defaultRpm = 0;
	//double defaultAcc = 0;
	//double unitRatio = 0;
	//double minAcc = 0;
	//bool reverse = 0;
};

