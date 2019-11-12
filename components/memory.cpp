//#include <fstream>
#include <string.h>
//#include <iostream>
//#include <iomanip>
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
	pMemory = new uint8_t[kMemorySize];
	
	memset((void*)(pMemory), 0xff, kMemorySize);
	memorySize = kMemorySize;

	LOGINFO("Memory initialised\n");

	return kError_OK;
}

void Memory::SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode Memory::Destroy(void)
{
	memorySize = 0;
	LOGINFO("Memory destroyed\n");

	return kError_OK;
}

/*
eErrorCode Memory::Load(const std::string& filename, uint16_t address, uint16_t* bytesRead)
{
	std::ifstream inFile;
	size_t size = 0;
	inFile.open(filename, std::ios::in | std::ios::binary);
	
	if(inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		size = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		inFile.read((char*)(pMemory + address), size);		
        inFile.close();
		*bytesRead = size;
	}
	else
	{
		// File load error
		LOGERROR("ERROR - cannot load file");
		return kError_FileNotFound;
	}

	return kError_OK;
}
*/

void Memory::Write(uint16_t address, uint8_t val)
{
	if(pMemory != 0)
	{
		pMemory[address] = val;
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
		return pMemory[address];
	}
	else
	{
		LOGERROR("Memory not initialised");
	}
	return 0;
}

