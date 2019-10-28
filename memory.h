#pragma once

#include "errorcodes.h"

#define MEMORY_SIZE ((uint16_t)65536)

// This is used to classify which type of memory transaction a mnemonic performs
// which is useful when different things happen for read/write with memory mapped hardware
enum eMemoryTransaction {
	kMemoryRead,
	kMemoryWrite
};

// Lifecycle
extern errorcode_t memory_Init(void);
extern errorcode_t memory_Destroy(void);

extern errorcode_t memory_Load(const char* filename, uint16_t address, uint16_t* bytesRead);


extern void memory_Write(uint16_t address, uint8_t val);
extern uint8_t memory_Read(uint16_t address);

// Debug stuff
extern void memory_DumpToTTY(uint16_t startAddress, uint16_t length);


