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
	pMemory = new MemoryByte[kMemorySize];
	memorySize = kMemorySize;

	pRam = new MemoryByte[kRamSize];
	ramSize = kRamSize;

	LOGINFO("Memory::Initialised\n");

	return kError_OK;
}

void Memory::SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode Memory::Destroy(void)
{
	memorySize = 0;
	LOGINFO("Memory::Destroyed\n");

	return kError_OK;
}

void Memory::Write(uint16_t address, uint8_t val, bool affectFlags)
{
	if(pMemory != 0)
	{
		pMemory[address].value = val;
		if(affectFlags)
		{
			pMemory[address].flags |= kMemoryFlagWrittenTo;
		}
	}
	else
	{
		LOGERROR("Memory not initialised");
	}
}

uint8_t Memory::Read(uint16_t address, bool affectFlags) const
{
//	uint16_t phsicalAddress = address & kAddressMask;

	if(pMemory != 0)
	{
		if(affectFlags)
		{
			pMemory[address].flags |= kMemoryFlagReadFrom;
		}
		return pMemory[address].value;
	}
	else
	{
		LOGERROR("Memory not initialised");
	}
	return 0;
}

uint8_t Memory::GetFlag(uint16_t address)
{
	if(pMemory != 0)
	{
		return pMemory[address].flags;
	}
	else
	{
		LOGERROR("Memory not initialised");
	}
	return 0;	
}

void Memory::SetReadBreakpoint(uint16_t address)
{
	pMemory[address].flags |= kMemoryFlagReadBreakpoint;
}

void Memory::SetWriteBreakpoint(uint16_t address)
{
	pMemory[address].flags |= kMemoryFlagWriteBreakpoint;
}

void Memory::ClearReadBreakpoint(uint16_t address)
{
	pMemory[address].flags &= 0xff ^ kMemoryFlagReadBreakpoint;
}

void Memory::ClearWriteBreakpoint(uint16_t address)
{
	pMemory[address].flags &= 0xff ^ kMemoryFlagWriteBreakpoint;
}
