#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "log.h"

static uint8_t* pMemory = 0;

errorcode_t memory_Init(void)
{
	if(pMemory != 0)
	{
		printf("ERROR - memory already initialised\n");
		return ERROR_MEMORY_ALREADY_INITIALISED;
	}

	pMemory = malloc(MEMORY_SIZE);
	memset(pMemory, MEMORY_SIZE, 0xff);

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
	LOG("Memory destroyed\n");

	return ERROR_OK;
}

void memory_Write(uint16_t address, uint8_t val)
{
	if(pMemory != 0)
	{
		pMemory[address] = val;
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
