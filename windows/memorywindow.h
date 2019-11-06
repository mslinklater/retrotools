#pragma once

#include <inttypes.h>

class Memory;

class MemoryWindow
{
public:
	
	struct Line {	// TODO: Move this out in to a view or helper class
		char	address[5];		// memory address of line
		char	value[16][3];	// the 16 memory location values
		char	key[16];		// representative figure for each byte
	};
	
	MemoryWindow();
	virtual ~MemoryWindow();

	void SetMemory(Memory* mem);
	
	void Draw();
	
private:
	uint16_t	startAddress;
	uint16_t	length;
	Memory*		pMemory;
	
	char		startAddressText[5];
	char		lengthText[5];
};
