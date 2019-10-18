// Global configuration vars

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

static char* pLoadROMFilename;
static uint16_t	loadROMAddress;

void config_Init(void)
{
	pLoadROMFilename = 0;
}

void config_Destroy(void)
{
	if(pLoadROMFilename != 0)
	{
		free(pLoadROMFilename);
		pLoadROMFilename = 0;
	}
}

errorcode_t config_ParseCommandLine(int32_t argc, char* argv[])
{
	// check for -h or --help and if found print help

	for(int i=0 ; i<argc ; i++)
	{
		if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
		{
			printf("Printing help\n");
			exit(0);
		}
	}

	// Check if config file is requested
	for(int i=0 ; i<argc ; i++)
	{
		if((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--config") == 0))
		{
		}

		if((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--load") == 0))
		{
			printf("Config:Found load command\n");
			if(argc <= i+2)
			{
				printf("ERROR - not passing correct -l params\n");
				return ERROR_COMMAND_LINE_PARSING;
			}

			// grab the filename
			pLoadROMFilename = argv[i+1];
			printf("Config:load filename %s\n", pLoadROMFilename);

			char* address = argv[i+2];
			loadROMAddress = strtol(address, NULL, 16);
			printf("Config:Address %d\n", loadROMAddress);
		}

	}

	// Now go through the arguments and set up the config

	return ERROR_OK;
}

char* config_GetLoadFilename(void)
{
	return pLoadROMFilename;
}

uint16_t config_GetLoadAddress(void)
{
	return loadROMAddress;
}

