#ifndef PARAMETER_H
#define PARAMETER_H

#include "stdint.h"
#include <string>

/*
 * Class representing a device parameter,
 * for example working mode
 */
class Parameter
{
public:
    enum Type : unsigned char
    {
        Int = 0,
        FracPoint1,
        FracPoint2,
        FracPoint3,
        FracPoint4,
        Hex
    };
    Parameter();
    Parameter(const std::string& name, const std::string& desc, const std::string& units, double value, double min, double max, Type type = Type::Int);
    Parameter& operator=(double d);
    const std::string& name() const;
    const std::string& description() const;
    const std::string& unit() const;
    Type getType() const;
    double getValue() const;
    double getRangeMin() const;
    double getRangeMax() const;
    size_t byteSize() const;
private:
    std::string _name, _description, _unit;
    Type _type;
    double _value, rangeMin, rangeMax;
};

#endif // PARAMETER_H
