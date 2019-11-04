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

/*
void Memory::DumpToTTY(uint16_t startAddress, uint16_t length)
{
	uint16_t length16 = ((length + 15) / 16) * 16;

	uint32_t endAddress = (uint32_t)startAddress + (uint32_t)length16;

	for(uint32_t currentAddress = (uint32_t)startAddress ; currentAddress < endAddress ; currentAddress += 16)
	{
		char output[80];
		sprintf(output, "0x%04x", (uint16_t)currentAddress);
		
		for(int i=0 ; i<16 ; i++)
		{
			char line[10];
			sprintf(line, " %02x", pMemory[currentAddress + i]);
			strcat(output, line);
		}	

		strcat(output,"  ");
		
		for(int i=0 ; i<16 ; i++)
		{
			uint8_t opcode = pMemory[currentAddress + i];
			
			if( pCpu->GetOpcode(opcode)->valid )
			{
				strcat(output,"O");
			}
			else
			{
				strcat(output,".");
			}
		}
		LOGINFO(output);
	}
}
*/

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
	return;
	lines.clear();
	for(int location = 0 ; location < MEMORY_SIZE ; location += 16)
	{
		char buffer[32];
		MemoryLine newLine;
		
		// Address
		sprintf(buffer, "%04x", (uint16_t)location);
		memcpy(&newLine.address, buffer, 4);
		
		// Bytes
		for(int iByte=0 ; iByte<16 ; iByte++)
		{
			sprintf(buffer, "%02x", pMemory[(location*16)+iByte]);
			memcpy(&newLine.value[iByte], buffer, 2);
		}
		
		// Keys
		for(int iByte=0 ; iByte<16 ; iByte++)
		{
			const Cpu6502::Opcode* opcode = pCpu->GetOpcode(pMemory[(location*16)+iByte]);
			if(opcode->valid)
			{
				newLine.key[iByte] = 'O';
			}
			else
			{
				newLine.key[iByte] = '.';
			}
		}
		lines.push_back(newLine);
	}
}
