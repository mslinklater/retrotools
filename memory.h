#pragma once

#include <cstdint>
#include <string>

#include "errorcodes.h"

#define MEMORY_SIZE 65536

class Cpu6502;

// This is used to classify which type of memory transaction a mnemonic performs
// which is useful when different things happen for read/write with memory mapped hardware

class Cpu6502;

class Memory
{
public:
	enum eTransactionType {
		kMemoryRead,
		kMemoryWrite
	};
	
	Memory();
	virtual ~Memory();
	
	eErrorCode 	Init();
	void 		SetCPU(Cpu6502* cpu);
	eErrorCode 	Destroy(void);
	eErrorCode 	Load(const std::string& filename, uint16_t address, uint16_t* bytesRead);
	uint8_t 	Read(uint16_t address) const;
	void 		Write(uint16_t address, uint8_t val);
	void 		DumpToTTY(uint16_t startAddress, uint16_t length);

private:
	uint8_t* 	pMemory;
	uint32_t	memorySize;
};
