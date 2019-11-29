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
	
	enum eTransactionType {
		kMemoryRead,
		kMemoryWrite
	};
	
	Memory();
	virtual ~Memory();
	
	eErrorCode 	Init();
	void 		SetCPU(Cpu6502* cpu);
	eErrorCode 	Destroy(void);
//	eErrorCode 	Load(const std::string& filename, uint16_t address, uint16_t* bytesRead);
	uint8_t 	Read(uint16_t address) const;
	void 		Write(uint16_t address, uint8_t val);

private:
	uint8_t* 	pMemory;
	uint32_t	memorySize;
	//uint16_t	addressMask;
};
