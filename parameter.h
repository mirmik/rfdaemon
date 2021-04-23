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
    Parameter(const std::string& name, double value);
    Parameter& operator=(double d);
    const std::string& name() const;
    double getValue() const;
    void setValue(double value);
    size_t byteSize() const;
private:
    std::string _name;
    double _value;
};

#endif // PARAMETER_H
