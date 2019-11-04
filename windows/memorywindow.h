#pragma once

#include <inttypes.h>

class Memory;

class MemoryWindow
{
public:
	MemoryWindow();
	virtual ~MemoryWindow();

	void SetMemory(Memory* mem);
	
	void Draw();
private:
	uint16_t	startAddress;
	Memory*		pMemory;
};
