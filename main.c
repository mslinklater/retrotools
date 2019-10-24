#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "config.h"
#include "memory.h"
#include "cpu.h"
#include "vcs.h"

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
	vcs_Init();
	cpu_Init();
	cpu_DumpInfo();

	// need to grab the ROM filename from the config settings
	if(config_GetLoadFilename() != 0)
	{
		uint16_t bytesLoaded;
		uint16_t loadAddress = config_GetLoadAddress();
		memory_Load(config_GetLoadFilename(), loadAddress, &bytesLoaded);
		memory_DumpToTTY(loadAddress, bytesLoaded);
		cpu_dumpDisassembly(loadAddress, bytesLoaded);
	}


	memory_Destroy();

	printf("Exiting...\n");
	initscr();
	endwin();
	return 0;
}
