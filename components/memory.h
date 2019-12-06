// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "../errorcodes.h"

/*
2600 memory layout

0x0000-0x002c	VIA		XXX0 0000 001X XXXX
0x0080-0x00ff	RAM		XXX0 0000 1XXX XXXX
0x0280-0x0297	RIOT	XXX0 001X XXXX XXXX
0x1000-0x1fff	ROM		XXX1 XXXX XXXX XXXX

bank mask values work out the bank offset
bank xor filters work out if that bank is selected

Addr 			= 0b101000000110101
Masked 			= 0b000000000110101
RAM Xor filter	= 0b000000000100000
XOR	& RAM Mask	= 0b000000000000000	YES - zero

Addr 			= 0b101000010110101
Masked 			= 0b000000010110101
RAM Xor filter	= 0b000000000100000
XOR	& RAM Mask	= 0b000000010000000	NO - non zero answer

*/

// This is used to classify which type of memory transaction a mnemonic performs
// which is useful when different things happen for read/write with memory mapped hardware

class Cpu6502;

class Memory
{
public:

	static const uint16_t kAddressMask = 0x1fff;

	static const uint16_t kRamAddressMask = 0x007f;
//	static const uint16_t kRamXorFilter = 0b

	static const int kMemorySize = 65536;
	static const int kRamSize = 128;
	
	static const uint8_t kMemoryFlagReadFrom = 1 << 0;
	static const uint8_t kMemoryFlagWrittenTo = 1 << 1;
	static const uint8_t kMemoryFlagWriteBreakpoint = 1 << 2;
	static const uint8_t kMemoryFlagReadBreakpoint = 1 << 3;

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
	uint8_t 	Read(uint16_t address, bool affectFlags = true) const;
	void 		Write(uint16_t address, uint8_t val, bool affectFlags = true);
	uint8_t		GetFlag(uint16_t address);

	void	SetReadBreakpoint(uint16_t address);
	void	SetWriteBreakpoint(uint16_t address);
	void	ClearReadBreakpoint(uint16_t address);
	void	ClearWriteBreakpoint(uint16_t address);

private:
	MemoryByte* pMemory;
	uint32_t	memorySize;

	MemoryByte* pRam;
	uint32_t	ramSize;
};
