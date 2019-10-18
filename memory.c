#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "log.h"
#include "errorcodes.h"

static uint8_t* pMemory = 0;
static uint16_t memorySize = 0;

errorcode_t memory_Init(void)
{
	if(pMemory != 0)
	{
		printf("ERROR - memory already initialised\n");
		return ERROR_MEMORY_ALREADY_INITIALISED;
	}

	pMemory = malloc(MEMORY_SIZE);
	memset((void*)pMemory, 0xff, MEMORY_SIZE);
	memorySize = MEMORY_SIZE;

	LOG("Memory initialised\n");

	return ERROR_OK;
}

errorcode_t memory_Destroy(void)
{
	if(pMemory == 0)
	{
		printf("ERROR - memory not initialised\n");
		return ERROR_MEMORY_NOT_INITIALISED;
	}
	free(pMemory);
	pMemory = 0;
	memorySize = 0;
	LOG("Memory destroyed\n");

	return ERROR_OK;
}

errorcode_t memory_Load(const char* filename, uint16_t address)
{
	// TODO: Some error checking - make sure memory is there and can fit in the ROM

	// load from file in to memory
	FILE* hFile = fopen(filename, "r");
	if(hFile != 0)
	{
		fseek(hFile, 0L, SEEK_END);
		uint32_t fileSize = ftell(hFile);
		fseek(hFile, 0L, SEEK_SET);
		fread((void*)&pMemory[address], 1, fileSize, hFile);
		fclose(hFile);
	}
	else
	{
		printf("ERROR - Cannot load file %s\n", filename);
		return ERROR_FILE_NOT_FOUND;
	}

	return ERROR_OK;
}

void memory_Write(uint16_t address, uint8_t val)
{
	if(pMemory != 0)
	{
		pMemory[address] = val;
	}
}

void memory_DumpToTTY(uint16_t startAddress, uint16_t length)
{
	uint16_t length16 = ((length + 15) % 16) * 16;

	uint16_t currentAddress = startAddress;

	for(uint16_t currentAddress = startAddress ; currentAddress <= startAddress + length16 ; currentAddress+=16)
	{
		printf("0x%04x", currentAddress);
		for(int i=0 ; i<16 ; i++)
		{
			printf(" %02x", pMemory[currentAddress + i]);
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
