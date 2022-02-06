// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#include <string.h>
#include "system/common.h"
#include "memory2600.h"
#include "system/errorcodes.h"
#include "components/cpu6502/cpu6502alpha.h"
#include "tia.h"
#include "riot.h"

Memory2600::Memory2600()
{
	pRam = new MemoryByte[kRamSize];
	ramSize = kRamSize;

	pRom = new MemoryByte[kRomSize];
	romSize = kRomSize;

	LOGINFO("Memory2600::Initialised\n");
}

Memory2600::~Memory2600()
{
	delete [] pRam;
	pRam = 0;

	delete [] pRom;
	pRom = 0;

	LOGINFO("Memory2600::Destroyed\n");
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
	if(address & kRomAddressFilter)
	{
		pRom[address & kRomAddressMask].value = val;
	}
	else
	{
		WriteImpl(address, val, false);
	}
}

void Memory2600::WriteImpl(uint16_t address, uint8_t val, bool affectFlags)
{
	// RIOT
	if(address & kRiotAddressFilter)
	{
		pRiot->Write(address & kRiotAddressMask, val);
	}
	// RAM
	else if(address & kRamAddressFilter)
	{
		uint16_t ramAddress = address & kRamAddressMask;
		if(affectFlags)
		{
			pRam[ramAddress].flags |= kMemoryFlagWrittenTo;
		}
		pRam[ramAddress].value = val;
	}
	// TIA
	else
	{
		pTia->Write(address & kTiaAddressMask, val);
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

	if(pByte != nullptr)
	{
		for(int i=0 ; i<numBytes ; i++)
		{
			pByte->flags |= kMemoryFlagHasBeenExecuted;
			pByte++;
		}
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

