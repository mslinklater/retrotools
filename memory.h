#pragma once

#include <cstdint>
#include <string>

#include "errorcodes.h"

#define MEMORY_SIZE ((uint16_t)65536)

class Cpu6502;

// This is used to classify which type of memory transaction a mnemonic performs
// which is useful when different things happen for read/write with memory mapped hardware
enum eMemoryTransaction {
	kMemoryRead,
	kMemoryWrite
};

// Lifecycle
extern eErrorCode memory_Init(void);
extern eErrorCode memory_Destroy(void);

extern eErrorCode memory_Load(const std::string& filename, uint16_t address, uint16_t* bytesRead);

extern void memory_Write(uint16_t address, uint8_t val);
extern uint8_t memory_Read(uint16_t address);

// Debug stuff
extern void memory_DumpToTTY(uint16_t startAddress, uint16_t length);
extern void memory_SetCPU(Cpu6502* cpu);



