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
	
	memset((void*)(pMemory), 0xff, kMemorySize);
	memorySize = kMemorySize;

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

void Memory::Write(uint16_t address, uint8_t val)
{
	if(pMemory != 0)
	{
		pMemory[address].value = val;
		pMemory[address].flags |= kMemoryFlagWrittenTo;
	}
	else
	{
		LOGERROR("Memory not initialised");
	}
}

uint8_t Memory::Read(uint16_t address) const
{
	if(pMemory != 0)
	{
		return pMemory[address].value;
		pMemory[address].flags |= kMemoryFlagReadFrom;
	}
	else
	{
		LOGERROR("Memory not initialised");
	}
	return 0;
}

