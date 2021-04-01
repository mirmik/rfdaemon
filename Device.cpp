#include "Device.h"

using namespace std;

Device::Device(const string& name, const std::string& type, const vector<Parameter>& parameters)
{
    _name = name;
    _parameters = parameters;
    _type = type;
}

const string& Device::getName() const
{
    return _name;
}

double Device::axisPosition()
{
    return axisPos;
}

const vector<Parameter>& Device::getParameters() const
{
    return _parameters;
}
