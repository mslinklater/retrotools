// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "../errorcodes.h"
#include "../config.h"
#include "../interfaces/imemory.h"

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
class Tia;

class Memory2600 : public IConfigSerialisation, public IMemory
{
public:

	// Limit caused by 6507 only having 13 address bus lines
	static const uint16_t kAddressMask = 0x1fff;

	// VIA
	static const uint16_t kViaStart = 0x0000;
	static const uint16_t kViaSize = 0x002d;

	// ROM
	static const uint16_t kRamStart = 0x0080;
	static const uint16_t kRamSize = 0x0080;

	// RIOT
	static const uint16_t kRiotStart = 0x0280;
	static const uint16_t kRiotSize = 0x0018;

	// ROM
	static const uint16_t kRomStart = 0x1000;
	static const uint16_t kRomSize = 0x1000;

	// Memory Byte flags
	static const uint8_t kMemoryFlagReadFrom = 1 << 0;
	static const uint8_t kMemoryFlagWrittenTo = 1 << 1;
	static const uint8_t kMemoryFlagWriteBreakpoint = 1 << 2;
	static const uint8_t kMemoryFlagReadBreakpoint = 1 << 3;
	static const uint8_t kMemoryFlagHasBeenExecuted = 1 << 4;

	enum eTransactionType {
		kMemoryRead,
		kMemoryWrite
	};
	
	struct MemoryByte
	{
		MemoryByte()
		: value(0)
		, flags(0)
		{}

		uint8_t	value;
		uint8_t flags;
	};

	Memory2600();
	virtual ~Memory2600();
	
	eErrorCode 	Init();
	void 		SetCPU(Cpu6502* cpu);
	void 		SetTia(Tia* tia);
	eErrorCode 	Destroy(void);

	// IMemory
	virtual uint8_t 	Read(uint16_t address);
	virtual void 		Write(uint16_t address, uint8_t val);
	virtual uint8_t 	DbgRead(uint16_t address);
	virtual void 		DbgWrite(uint16_t address, uint8_t val);
	virtual void		SetHasBeenExecuted(uint16_t address, uint16_t numBytes);
	// ~IMemory

	uint8_t		GetFlag(uint16_t address);

	void	SetReadBreakpoint(uint16_t address);
	void	SetWriteBreakpoint(uint16_t address);
	void	ClearReadBreakpoint(uint16_t address);
	void	ClearWriteBreakpoint(uint16_t address);

	// IConfigSerialisation
	void SerialiseState(json& object);
	void DeserialiseState(json& object);
	// ~IConfigSerialisation

private:
	MemoryByte* pRom;
	uint32_t	romSize;

	MemoryByte* pRam;
	uint32_t	ramSize;

	Tia*	pTia;

	uint8_t 	ReadImpl(uint16_t address, bool affectFlags);
	void 		WriteImpl(uint16_t address, uint8_t val, bool affectFlags);
};