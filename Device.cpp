#include "Device.h"
#include "Axis.h"

using namespace std;

Device::Device(const string& name, Type type)
{
    _name = name;
    _type = type;
}

const string& Device::name() const
{
    return _name;
}

Axis* Device::axis() const
{
    return _axis;
}

Device::Type Device::type() const
{
    return _type;
}

double Device::sensorValue() const
{
    return _sensorValue;
}

void Device::setAxis(Axis* axis)
{
    _axis = axis;
}

void Device::updateSensorValue(double value)
{
    _sensorValue = value;
}
