// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "../errorcodes.h"

// This is used to classify which type of memory transaction a mnemonic performs
// which is useful when different things happen for read/write with memory mapped hardware

class Cpu6502;

class Memory
{
public:
	static const int kMemorySize = 65536;
	
	static const uint8_t kMemoryFlagReadFrom = 1 << 0;
	static const uint8_t kMemoryFlagWrittenTo = 1 << 1;

	enum eTransactionType {
		kMemoryRead,
		kMemoryWrite
	};
	
	struct MemoryByte
	{
		MemoryByte()
		: flags(0)
		{}

		uint8_t	value;
		uint8_t flags;
	};

	Memory();
	virtual ~Memory();
	
	eErrorCode 	Init();
	void 		SetCPU(Cpu6502* cpu);
	eErrorCode 	Destroy(void);
	uint8_t 	Read(uint16_t address) const;
	void 		Write(uint16_t address, uint8_t val);

private:
	MemoryByte* pMemory;
	uint32_t	memorySize;
	//uint16_t	addressMask;
};
