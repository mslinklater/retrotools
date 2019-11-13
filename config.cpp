// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

// Global configuration vars

#include <string.h>
#include <string>
#include <cstdint>

#include "config.h"
#include "log.h"


Config::Config()
{
}

void Config::Init(void)
{
}

void Config::Destroy(void)
{
}

eErrorCode Config::ParseCommandLine(int32_t argc, char* argv[])
{
	// check for -h or --help and if found print help

	for(int i=0 ; i<argc ; i++)
	{
		if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
		{
			LOGINFO("Printing help\n");
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
			LOGINFO("Config:Found load command\n");
			if(argc <= i+2)
			{
				LOGERROR("ERROR - not passing correct -l params\n");
				return kError_CommandLineParsing;
			}

			// grab the filename
			loadROMFilename = argv[i+1];
//			Log::Instance()->Infof("Plap %d", 10);
//			LOGINFO("Config:load filename %s\n", loadROMFilename.c_str());

			char* address = argv[i+2];
			loadROMAddress = strtol(address, NULL, 16);
			LOGINFOF("Config:Address %d\n", loadROMAddress);
		}

	}

	// Now go through the arguments and set up the config

	return kError_OK;
}

std::string Config::GetLoadFilename(void)
{
	return loadROMFilename;
}

uint16_t Config::GetLoadAddress(void)
{
	return loadROMAddress;
}
