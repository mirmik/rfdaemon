#include "parameter.h"

using namespace std;

Parameter::Parameter(const string& name, double value)
{
    _name = name;
    _value = value;
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

double Parameter::getValue() const
{
    return _value;
}

void Parameter::setValue(double value)
{
    _value = value;
}

size_t Parameter::byteSize() const
{
    return sizeof(_value) + _name.size() + 1;
}
