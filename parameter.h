#ifndef PARAMETER_H
#define PARAMETER_H

#include "stdint.h"
#include <string>

class Parameter
{
public:
    Parameter();
    Parameter(const std::string& name, const std::string& desc, const std::string& units, int64_t min, int64_t max, int64_t value, bool hex = false);
    Parameter& operator=(int64_t d);
    std::string name() const;
    std::string description() const;
    std::string unit() const;
    bool getIsHex() const;
    int64_t getValue() const;
    int64_t getRangeMin() const;
    int64_t getRangeMax() const;
    size_t byteSize() const;
private:
    std::string _name, _description, _unit;
    bool isHex;
    int64_t _value, rangeMin, rangeMax;
};

#endif // PARAMETER_H
