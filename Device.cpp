#include "Device.h"
#include "Axis.h"

using namespace std;

Device::Device(const string& name, Type type, const vector<Parameter>& parameters)
{
    _name = name;
    _parameters = parameters;
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

const vector<Parameter>& Device::parameters() const
{
    return _parameters;
}

void Device::setParameterValue(uint16_t id, double value)
{
    if (_parameters.size() > id)
        _parameters[id].setValue(value);
    else
        printf("Unknown parameter id: %d\n", id);
}
