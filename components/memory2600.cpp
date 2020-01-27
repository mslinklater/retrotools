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
#include "riot.h"

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

eErrorCode Memory2600::Destroy(void)
{
	LOGINFO("Memory2600::Destroyed\n");

	delete [] pRam;
	pRam = 0;

	delete [] pRom;
	pRom = 0;

	return kError_OK;
}

void Memory2600::SetCPU(Cpu6502* cpu)
{
	pCpu = cpu;
}
void Memory2600::SetTia(Tia* tia)
{
	pTia = tia;
}
void Memory2600::SetRiot(Riot* riot)
{
	pRiot = riot;
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
	uint16_t lowMaskPhysicalAddress = physicalAddress & 0xfeff;

	if((lowMaskPhysicalAddress >= kTiaStart) && (lowMaskPhysicalAddress < kTiaStart + kTiaSize))
	{
		pTia->Write(lowMaskPhysicalAddress, val);
	}
	else if((lowMaskPhysicalAddress >= kRamStart) && (lowMaskPhysicalAddress < kRamStart + kRamSize))
	{
		// RAM write
		uint16_t ramAddress = lowMaskPhysicalAddress - kRamStart;
		pRam[ramAddress].value = val;
		if(affectFlags)
		{
			pRam[ramAddress].flags |= kMemoryFlagWrittenTo;
		}
	}
	else if((physicalAddress >= kRiotStart) && (physicalAddress < kRiotStart + kRiotSize))
	{
		pRiot->Write(physicalAddress, val);
	}
	else if((physicalAddress >= kRomStart) && (physicalAddress < kRomStart + kRomSize))
	{
		// ROM write - not really possible - fix this !
		uint16_t romAddress = physicalAddress - kRomStart;
		pRom[romAddress].value = val;
		pRom[romAddress].flags = 0;
	}
	else
	{
//		LOGERRORF("Unhandled memory write 0x%04x", address);
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
	// ROM
	if(address & kRomAddressFilter)
	{
		uint16_t romAddress = address & kRomAddressMask;
		if(affectFlags)
		{
			pRom[romAddress].flags |= kMemoryFlagReadFrom;
		}
		return pRom[romAddress].value;
	}
	// RIOT
	else if(address & kRiotAddressFilter)
	{
		return pRiot->Read(address & kRiotAddressMask);
	}
	// RAM
	else if(address & kRamAddressFilter)
	{
		uint16_t ramAddress = address & kRamAddressMask;
		if(affectFlags)
		{
			pRam[ramAddress].flags |= kMemoryFlagReadFrom;
		}
		return pRam[ramAddress].value;
	}
	// TIA
	else
	{
		return pTia->Read(address & kTiaAddressMask);
	}
	return 0;
}

uint8_t Memory2600::GetFlag(uint16_t address)
{
	// ROM
	if(address & kRomAddressFilter)
	{
		return pRom[address & kRomAddressMask].flags;
	}
	// RIOT
	else if(address & kRiotAddressFilter)
	{
	}
	// RAM
	else if(address & kRamAddressFilter)
	{
		return pRam[address & kRamAddressMask].flags;
	}
	// TIA
	else
	{
	}

	return 0;	
}

void Memory2600::SetHasBeenExecuted(uint16_t address, uint16_t numBytes)
{
	MemoryByte* pByte = nullptr;

	if(address & kRomAddressFilter)
	{
		pByte = pRom + (address & kRomAddressMask);
	}
	else if(address & kRamAddressFilter)
	{
		pByte = pRam + (address & kRamAddressMask);
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

