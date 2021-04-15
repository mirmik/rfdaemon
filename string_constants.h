#pragma once

#include "Device.h"

enum ParamGroup
{
	PA = 0,
	PB,
	PC,
	PD,
	PE,
	PF
};

extern const char* const * const * const devtypeA_PA;
extern const char* const * const * const devtypeA_PB;
extern const char* const * const * const devtypeA_PC;
extern const char* const * const * const devtypeA_PD;
extern const char* const * const * const devtypeA_PE;
extern const char* const * const * const devtypeA_PF;
extern const char* const * const * const devtypeB_PA;
extern const char* const * const * const devtypeB_PB;
extern const char* const * const * const devtypeB_PC;
extern const char* const * const * const devtypeB_PD;
extern const char* const * const * const devtypeB_PE;
extern const char* const * const * const devtypeB_PF;

extern const char devtypeA_PA_IDList[];
extern const char devtypeA_PB_IDList[];
extern const char devtypeA_PC_IDList[];
extern const char devtypeA_PD_IDList[];
extern const char devtypeA_PE_IDList[];
extern const char devtypeA_PF_IDList[];
extern const char devtypeB_PA_IDList[];
extern const char devtypeB_PB_IDList[];
extern const char devtypeB_PC_IDList[];
extern const char devtypeB_PD_IDList[];
extern const char devtypeB_PE_IDList[];
extern const char devtypeB_PF_IDList[];

constexpr unsigned int devtypeA_PA_size = 26;
constexpr unsigned int devtypeA_PB_size = 52;
constexpr unsigned int devtypeA_PC_size = 48;
constexpr unsigned int devtypeA_PD_size = 37;
constexpr unsigned int devtypeA_PE_size = 8;
constexpr unsigned int devtypeA_PF_size = 11;
constexpr unsigned int devtypeB_PA_size = 18;
constexpr unsigned int devtypeB_PB_size = 51;
constexpr unsigned int devtypeB_PC_size = 25;
constexpr unsigned int devtypeB_PD_size = 15;
constexpr unsigned int devtypeB_PE_size = 9;
constexpr unsigned int devtypeB_PF_size = 9;

const char** getDefaultParam(Device::Type type, ParamGroup group, int paramId);
