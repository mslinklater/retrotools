#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "memory.h"

int main(int argc, char* argv[])
{
	printf("-- retrotool --\n");

	// check for command line args
	
	if(argc == 1)
	{
		printf("No command line args specified, exiting...\n");
		exit(0);
	}

	config_Init();
	if(config_ParseCommandLine(argc, argv) != ERROR_OK)
	{
		printf("Error parsing command line... aborting \n");
		exit(1);
	}

	// initialise a simple 6502 machine
	
	memory_Init();

	// need to grab the ROM filename from the config settings

	memory_Destroy();

	printf("Exiting...\n");
	return 0;
}
