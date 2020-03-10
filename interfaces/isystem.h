#pragma once

#include <inttypes.h>

class ISystem
{
public:
	virtual uint8_t CpuRead(uint16_t address) = 0;
	virtual void CpuWrite(uint16_t address, uint8_t value) = 0;
};
