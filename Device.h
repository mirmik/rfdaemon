#pragma once

#include <vector>
#include <string>
#include "parameter.h"

class Device
{
public:
	Device(const std::string& name, const std::string& type, const std::vector<Parameter>& parameters);
	const std::vector<Parameter>& getParameters() const;
	const std::string& getName() const;
private:
	std::string _name;
	std::string _type;
	std::vector<Parameter> _parameters;
};
