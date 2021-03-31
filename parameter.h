#ifndef PARAMETER_H
#define PARAMETER_H

#include "stdint.h"
#include <string>

class Parameter
{
public:
    Parameter();
    Parameter(const std::string& name, const std::string& desc, const std::string& units, double min, double max, double value, bool hex = false);
    Parameter& operator=(double d);
    const std::string& name() const;
    const std::string& description() const;
    const std::string& unit() const;
    bool getIsHex() const;
    double getValue() const;
    double getRangeMin() const;
    double getRangeMax() const;
    size_t byteSize() const;
private:
    std::string _name, _description, _unit;
    bool isHex;
    double _value, rangeMin, rangeMax;
};

#endif // PARAMETER_H
