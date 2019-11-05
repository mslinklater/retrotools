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
	void Update();
	
private:
	uint16_t	startAddress;
	uint16_t	length;
	Memory*		pMemory;
	
	char		startAddressText[5];
	char		lengthText[5];
};
