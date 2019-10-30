//#include <stdlib.h>
//#include <stdio.h>
#include <fstream>
#include <string.h>
#include "memory.h"
#include "log.h"
#include "errorcodes.h"
#include "cpu.h"

static char* pMemory;
static uint32_t memorySize = 0;

static Cpu6502* pCpu;

eErrorCode memory_Init(void)
{
	pMemory = new char[MEMORY_SIZE];
	
	memset((void*)(pMemory), 0xff, MEMORY_SIZE);
	memorySize = MEMORY_SIZE;

	printf("Memory initialised\n");

	return kError_OK;
}

void memory_SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode memory_Destroy(void)
{
	memorySize = 0;
	LOG("Memory destroyed\n");

	return kError_OK;
}

eErrorCode memory_Load(const std::string& filename, uint16_t address, uint16_t* bytesRead)
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
	}

	return kError_OK;
}

void memory_Write(uint16_t address, uint8_t val)
{
	pMemory[address] = val;
}

void memory_DumpToTTY(uint16_t startAddress, uint16_t length)
{
	uint16_t length16 = ((length + 15) / 16) * 16;

	uint16_t currentAddress = startAddress;

	for(uint16_t currentAddress = startAddress ; currentAddress < startAddress + length16 ; currentAddress+=16)
	{
		printf("0x%04x", currentAddress);
		for(int i=0 ; i<16 ; i++)
		{
			printf(" %02x", pMemory[currentAddress + i]);
		}	

		printf("  ");
		for(int i=0 ; i<16 ; i++)
		{
			uint8_t operand = pMemory[currentAddress + i];
			if( pCpu->IsOpcodeValid(operand) )
			{
				printf("O");
			}
			else
			{
				printf(".");
			}
		}
		printf("\n");
	}
}

uint8_t memory_Read(uint16_t address)
{
//	if(pMemory != 0)
//	{
		return pMemory[address];
//	}
	return 0;
}
