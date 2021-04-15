#include "Axis.h"
#include "Device.h"

using namespace std;

Axis::Axis(const string& name, Type type, Motion motionType)
{
	_name = name;
	_type = type;
	_motionType = motionType;
}

const std::string& Axis::name() const
{
	return _name;
}

void Axis::linkDevice(Device* dev)
{
	linkedDevices.push_back(dev);
}

void Axis::linkDevices(const vector<Device*>& devs)
{
	if (linkedDevices.empty())
		linkedDevices = devs;
	else
		linkedDevices.insert(linkedDevices.end(), devs.begin(), devs.end());
}

void Axis::setSyncDevice(Device* dev)
{
	syncDevice = dev;
}

Device* Axis::syncDev() const
{
	return syncDevice;
}

Axis::Type Axis::type() const
{
	return _type;
}

Axis::Motion Axis::motionType() const
{
	return _motionType;
}

double Axis::position() const
{
	return pos;
}

double Axis::minLimit() const
{
	return _minLimit;
}

double Axis::maxLimit() const
{
	return _maxLimit;
}

const vector<Device*>& Axis::linkedDevs() const
{
	return linkedDevices;
}

void Axis::setMinLimit(double value)
{
	_minLimit = value;
}

void Axis::setMaxLimit(double value)
{
	_maxLimit = value;
}

void Axis::setPosition(double value)
{
	pos = value;
}
