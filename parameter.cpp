#include "parameter.h"

using namespace std;

Parameter::Parameter()
{
    _name = "";
    _description = "";
    _unit = "";
    isHex = 0;
    _value = 0;
    rangeMin = 0;
    rangeMax = 0;
}

Parameter::Parameter(const string& name, const string& desc, const string& units, double min, double max, double value, bool hex)
{
    _name = name;
    _description = desc;
    _unit = units;
    isHex = hex;
    _value = value;
    rangeMin = min;
    rangeMax = max;
}

Parameter &Parameter::operator=(double d)
{
    _value = d;
    return *this;
}

const string& Parameter::name() const
{
    return _name;
}

const string& Parameter::description() const
{
    return _description;
}

const string& Parameter::unit() const
{
    return _unit;
}

bool Parameter::getIsHex() const
{
    return isHex;
}

double Parameter::getValue() const
{
    return _value;
}

double Parameter::getRangeMin() const
{
    return rangeMin;
}

double Parameter::getRangeMax() const
{
    return rangeMax;
}

size_t Parameter::byteSize() const
{
    return sizeof(_value) + sizeof(rangeMin) + sizeof(rangeMax) + _name.size() + _description.size() + _unit.size() + 4;
}
