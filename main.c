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

//	config_parse_command_line(argc, argv);

	return 0;
}
