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

	LOG("Memory initialised\n");

	return kError_OK;
}

void Memory::SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode Memory::Destroy(void)
{
	memorySize = 0;
	LOG("Memory destroyed\n");

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

	return kError_OK;
}

void Memory::Write(uint16_t address, uint8_t val)
{
	pMemory[address] = val;
}

void Memory::DumpToTTY(uint16_t startAddress, uint16_t length)
{
	uint16_t length16 = ((length + 15) / 16) * 16;

	uint16_t currentAddress = startAddress;

	for(uint16_t currentAddress = startAddress ; currentAddress < startAddress + length16 ; currentAddress+=16)
	{
		std::cout << std::hex << currentAddress;
		
		for(int i=0 ; i<16 ; i++)
		{
			std::cout << " ";
			std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)pMemory[currentAddress+i];
		}	

		std::cout << "  ";
		for(int i=0 ; i<16 ; i++)
		{
			uint8_t opcode = pMemory[currentAddress + i];
			if( pCpu->GetOpcode(opcode)->valid )
			{
				std::cout << "O";
			}
			else
			{
				std::cout << ".";			}
		}
		std::cout << std::endl;
	}
}

uint8_t Memory::Read(uint16_t address) const
{
	if(pMemory != 0)
	{
		return pMemory[address];
	}
	return 0;
}
