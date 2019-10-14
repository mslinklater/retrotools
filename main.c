#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int main(int argc, char* argv[])
{
	printf("-- retrotool --\n");

	// check for command line args
	
	if(argc == 1)
	{
		printf("No command line args specified, exiting...\n");
		exit(0);
	}

	if(config_ParseCommandLine(argc, argv) != ERROR_OK)
	{
		printf("Error parsing command line... aborting \n");
		exit(1);
	}

	printf("Exiting...\n");
	return 0;
}
