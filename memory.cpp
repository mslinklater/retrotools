//#include <stdlib.h>
//#include <stdio.h>
#include <fstream>
#include <string.h>
#include "memory.h"
#include "log.h"
#include "errorcodes.h"
#include "cpu.h"

static uint8_t memory[MEMORY_SIZE];
static uint16_t memorySize = 0;

static Cpu6502* pCpu;

eErrorCode memory_Init(void)
{
//	if(pMemory != 0)
//	{
//		printf("ERROR - memory already initialised\n");
//		return kError_MemoryAlreadyInitialised;
//	}

//	pMemory = new uint8_t[MEMORY_SIZE];
	memset((void*)memory, 0xff, MEMORY_SIZE);
	memorySize = MEMORY_SIZE;

	LOG("Memory initialised\n");

	return kError_OK;
}

void memory_SetCPU(Cpu6502* cpu)
{
    pCpu = cpu;
}

eErrorCode memory_Destroy(void)
{
//	if(pMemory == 0)
//	{
//		printf("ERROR - memory not initialised\n");
//		return kError_MemoryNotInitialised;
//	}
//	delete [] pMemory;
//	pMemory = 0;
	memorySize = 0;
	LOG("Memory destroyed\n");

	return kError_OK;
}

eErrorCode memory_Load(const std::string& filename, uint16_t address, uint16_t* bytesRead)
{
	// TODO: Some error checking - make sure memory is there and can fit in the ROM

	// load from file in to memory
	/*
	FILE* hFile = fopen(filename.c_str(), "r");
	if(hFile != 0)
	{
		fseek(hFile, 0L, SEEK_END);
		uint32_t fileSize = ftell(hFile);
		fseek(hFile, 0L, SEEK_SET);
		*bytesRead = fread((void*)&pMemory[address], 1, fileSize, hFile);
		fclose(hFile);
		printf("Read %d bytes\n", *bytesRead);
	}
	else
	{
		printf("ERROR - Cannot load file %s\n", filename.c_str());
		return kError_FileNotFound;
	}
	*/
	
	std::ifstream inFile;
	size_t size = 0;
	inFile.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
	
	if(inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		size = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		inFile.read((char*)(&memory[address]), size);
	}

	return kError_OK;
}

void memory_Write(uint16_t address, uint8_t val)
{
//	if(pMemory != 0)
//	{
		memory[address] = val;
//	}
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
	if(pMemory != 0)
	{
		return pMemory[address];
	}
	return 0;
}
