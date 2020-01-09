// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <string.h>
#include "memory2600.h"
#include "../shared_cpp/log.h"
#include "../shared_cpp/errorcodes.h"
#include "cpu6502.h"
#include "tia.h"

static Cpu6502* pCpu;

Memory2600::Memory2600()
{
}

Memory2600::~Memory2600()
{
}
	
eErrorCode Memory2600::Init(void)
{
	pRam = new MemoryByte[kRamSize];
	ramSize = kRamSize;

	pRom = new MemoryByte[kRomSize];
	romSize = kRomSize;

	LOGINFO("Memory2600::Initialised\n");

	return kError_OK;
}

void Memory2600::SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode Memory2600::Destroy(void)
{
	LOGINFO("Memory2600::Destroyed\n");

	delete [] pRam;
	pRam = 0;

	delete [] pRom;
	pRom = 0;

	return kError_OK;
}

void Memory2600::Write(uint16_t address, uint8_t val)
{
	WriteImpl(address, val, true);
}

void Memory2600::DbgWrite(uint16_t address, uint8_t val)
{
	WriteImpl(address, val, false);
}

void Memory2600::WriteImpl(uint16_t address, uint8_t val, bool affectFlags)
{
	uint16_t physicalAddress = address & kAddressMask;

	if((physicalAddress >= kViaStart) && (physicalAddress < kViaStart + kViaSize))
	{
		pTia->Write(address, val);
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

uint8_t Memory2600::Read(uint16_t address)
{
	return ReadImpl(address, true);
}

uint8_t Memory2600::DbgRead(uint16_t address)
{
	return ReadImpl(address, false);
}

uint8_t Memory2600::ReadImpl(uint16_t address, bool affectFlags)
{
	uint16_t physicalAddress = address & kAddressMask;

	if((physicalAddress >= kViaStart) && (physicalAddress < kViaStart + kViaSize))
	{
		return pTia->Read(address);
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

uint8_t Memory2600::GetFlag(uint16_t address)
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

void Memory2600::SetHasBeenExecuted(uint16_t address, uint16_t numBytes)
{
	uint16_t physicalAddress = address & kAddressMask;

	MemoryByte* pByte = nullptr;

	if((physicalAddress >= kRamStart) && (physicalAddress < kRamStart + kRamSize))
	{
		uint16_t ramAddress = physicalAddress - kRamStart;

		pByte = pRam + ramAddress;
	}
	else
	{
		uint16_t romAddress = physicalAddress - kRomStart;
		pByte = pRom + romAddress;
	}

	for(int i=0 ; i<numBytes ; i++)
	{
		pByte->flags |= kMemoryFlagHasBeenExecuted;
		pByte++;
	}
}

void Memory2600::SerialiseState(json& object)
{
	LOGINFO("Memory2600::SerialiseState");
}

void Memory2600::DeserialiseState(json& object)
{
	LOGINFO("Memory2600::DeserialiseState");
}

void Memory2600::SetReadBreakpoint(uint16_t address)
{
}

void Memory2600::SetWriteBreakpoint(uint16_t address)
{
}

void Memory2600::ClearReadBreakpoint(uint16_t address)
{
}

void Memory2600::ClearWriteBreakpoint(uint16_t address)
{
}

void Memory2600::SetTia(Tia* tia)
{
	pTia = tia;
}