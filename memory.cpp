#include <fstream>
#include <string.h>
#include <iostream>
#include <iomanip>
#include "memory.h"
#include "log.h"
#include "errorcodes.h"
#include "cpu.h"

static Cpu6502* pCpu;

Memory::Memory()
{
}

Memory::~Memory()
{
}
	
eErrorCode Memory::Init(void)
{
	pMemory = new uint8_t[MEMORY_SIZE];
	
	memset((void*)(pMemory), 0xff, MEMORY_SIZE);
	memorySize = MEMORY_SIZE;

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
		LOGINFO("ERROR - cannot load file");
		return kError_FileNotFound;
	}

	PopulateLines();
	return kError_OK;
}

void Memory::Write(uint16_t address, uint8_t val)
{
	pMemory[address] = val;
}

uint8_t Memory::Read(uint16_t address) const
{
	if(pMemory != 0)
	{
		return pMemory[address];
	}
	return 0;
}

void Memory::PopulateLines()
{
	lines.clear();
	for(int location = 0 ; location < MEMORY_SIZE ; location += 16)
	{
		MemoryLine newLine;
		
		// Address
		sprintf(&(newLine.address[0]), "%04x", (uint16_t)location);
		
		// Bytes
		for(int iByte=0 ; iByte<16 ; iByte++)
		{
			sprintf(&(newLine.value[iByte][0]), "%02x", pMemory[location+iByte]);
		}
		
		// Keys
		for(int iByte=0 ; iByte<16 ; iByte++)
		{
			const Cpu6502::Opcode* opcode = pCpu->GetOpcode(pMemory[location+iByte]);

			if(opcode->valid)
			{
				newLine.key[iByte] = 'O';
			}
			else
			{
				newLine.key[iByte] = '.';
			}
		}
		newLine.key[16] = 0;
		lines.push_back(newLine);
	}
}

const Memory::MemoryLine& Memory::GetLineForAddress(uint16_t address)
{
	int whichLine = address / 16;
	return lines[whichLine];
}
