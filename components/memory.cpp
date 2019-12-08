// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <string.h>
#include "memory.h"
#include "../log.h"
#include "../errorcodes.h"
#include "cpu6502.h"

static Cpu6502* pCpu;

Memory::Memory()
{
}

Memory::~Memory()
{
}
	
eErrorCode Memory::Init(void)
{
	pRam = new MemoryByte[kRamSize];
	ramSize = kRamSize;

	pRom = new MemoryByte[kRomSize];
	romSize = kRomSize;

	LOGINFO("Memory::Initialised\n");

	return kError_OK;
}

void Memory::SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode Memory::Destroy(void)
{
	LOGINFO("Memory::Destroyed\n");

	delete [] pRam;
	pRam = 0;

	delete [] pRom;
	pRom = 0;

	return kError_OK;
}

void Memory::Write(uint16_t address, uint8_t val, bool affectFlags)
{
	uint16_t physicalAddress = address & kAddressMask;

	if((physicalAddress >= kViaStart) && (physicalAddress < kViaStart + kViaSize))
	{
		// VIA write
		LOGWARNINGF("Memory::VIA Write 0x%04x", physicalAddress);
	}
	else if((physicalAddress >= kRamStart) && (physicalAddress < kRamStart + kRamSize))
	{
		// RAM write
		uint16_t ramAddress = physicalAddress - kRamStart;
		pRam[ramAddress].value = val;
		if(affectFlags)
		{
			pRam[ramAddress].flags |= kMemoryFlagWrittenTo;
		}
	}
	else if((physicalAddress >= kRiotStart) && (physicalAddress < kRiotStart + kRiotSize))
	{
		// RIOT write
		LOGWARNINGF("Memory::RIOT Write 0x%04x", physicalAddress);
	}
	else if((physicalAddress >= kRomStart) && (physicalAddress < kRomStart + kRomSize))
	{
		// ROM write - not really possible - fix this !
		uint16_t romAddress = physicalAddress - kRomStart;
		pRom[romAddress].value = val;
		pRom[romAddress].flags = 0;
	}
}

uint8_t Memory::Read(uint16_t address, bool affectFlags) const
{
	uint16_t physicalAddress = address & kAddressMask;

	if((physicalAddress >= kViaStart) && (physicalAddress < kViaStart + kViaSize))
	{
		// VIA read
		LOGWARNINGF("Memory::VIA Read 0x%04x", physicalAddress);
	}
	else if((physicalAddress >= kRamStart) && (physicalAddress < kRamStart + kRamSize))
	{
		uint16_t ramAddress = physicalAddress - kRamStart;
		if(affectFlags)
		{
			pRam[ramAddress].flags |= kMemoryFlagReadFrom;
		}
		return pRam[ramAddress].value;
	}
	else if((physicalAddress >= kRiotStart) && (physicalAddress < kRiotStart + kRiotSize))
	{
		// RIOT read
		LOGWARNINGF("Memory::RIOT Read 0x%04x", physicalAddress);
	}
	else if((physicalAddress >= kRomStart) && (physicalAddress < kRomStart + kRomSize))
	{
		uint16_t romAddress = physicalAddress - kRomStart;
		if(affectFlags)
		{
			pRom[romAddress].flags |= kMemoryFlagReadFrom;
		}
		return pRom[romAddress].value;
	}

	return 0;
}

uint8_t Memory::GetFlag(uint16_t address)
{
	uint16_t physicalAddress = address & kAddressMask;

	if((physicalAddress >= kViaStart) && (physicalAddress < kViaStart + kViaSize))
	{
		// VIA read
	}
	else if((physicalAddress >= kRamStart) && (physicalAddress < kRamStart + kRamSize))
	{
		uint16_t ramAddress = physicalAddress - kRamStart;
		return pRam[ramAddress].flags;
	}
	else if((physicalAddress >= kRiotStart) && (physicalAddress < kRiotStart + kRiotSize))
	{
		// RIOT read
	}
	else if((physicalAddress >= kRomStart) && (physicalAddress < kRomStart + kRomSize))
	{
		uint16_t romAddress = physicalAddress - kRomStart;
		return pRom[romAddress].flags;
	}
	return 0;	
}

void Memory::SerialiseState(json& object)
{
	LOGINFO("Memory::SerialiseState");
}

void Memory::DeserialiseState(json& object)
{
	LOGINFO("Memory::DeserialiseState");
}

void Memory::SetReadBreakpoint(uint16_t address)
{
}

void Memory::SetWriteBreakpoint(uint16_t address)
{
}

void Memory::ClearReadBreakpoint(uint16_t address)
{
}

void Memory::ClearWriteBreakpoint(uint16_t address)
{
}
