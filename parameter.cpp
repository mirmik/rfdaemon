#include "parameter.h"

using namespace std;

Parameter::Parameter()
{
    _name = "";
    _description = "";
    _unit = "";
    _type = Type::Int;
    _value = 0;
    rangeMin = 0;
    rangeMax = 0;
}

Parameter::Parameter(const string& name, const string& desc, const string& units, double value, double min, double max, Type type)
{
    _name = name;
    _description = desc;
    _unit = units;
    _type = type;
    _value = value;
    rangeMin = min;
    rangeMax = max;
}

Parameter& Parameter::operator=(double d)
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

Parameter::Type Parameter::getType() const
{
    return _type;
}

double Parameter::getValue() const
{
    return _value;
}

void Parameter::setValue(double value)
{
    _value = value;
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
