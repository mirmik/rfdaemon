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

Parameter::Parameter(const string& name, const string& desc, const string& units, int64_t min, int64_t max, int64_t value, bool hex)
{
    _name = name;
    _description = desc;
    _unit = units;
    isHex = hex;
    _value = value;
    rangeMin = min;
    rangeMax = max;
}

Parameter &Parameter::operator=(int64_t d)
{
    _value = d;
    return *this;
}

string Parameter::name() const
{
    return _name;
}

string Parameter::description() const
{
    return _description;
}

string Parameter::unit() const
{
    return _unit;
}

bool Parameter::getIsHex() const
{
    return isHex;
}

int64_t Parameter::getValue() const
{
    return _value;
}

int64_t Parameter::getRangeMin() const
{
    return rangeMin;
}

int64_t Parameter::getRangeMax() const
{
    return rangeMax;
}

size_t Parameter::byteSize() const
{
    return sizeof(_value) + sizeof(rangeMin) + sizeof(rangeMax) + 1 + _name.size() + _description.size() + _unit.size();
}
