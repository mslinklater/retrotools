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

void Config::SerialiseAppConfig()
{
	LOGINFO("Config::SerialiseAppConfig");
	json outputJson;

	// grab info

	for(auto serialiser : serialisers)
	{
		serialiser->SerialiseState(outputJson);
	}

	std::string outputString = outputJson.dump();
	FILE* outputFile = fopen("config.json", "w");
	if(outputFile)
	{
		fwrite(outputString.c_str(), outputString.size(), 1, outputFile);
		fclose(outputFile);
	}
}

void Config::DeserialiseAppConfig()
{
	LOGINFO("Config::DeserialiseAppConfig");

	json inputJson;

	// Load from file.
	FILE* hFile = fopen("config.json", "r");
	if(hFile != 0)
	{
		fseek(hFile, 0, SEEK_END);
		size_t fileSize = ftell(hFile);
		fseek(hFile, 0, SEEK_SET);
		char* pBuffer = new char[fileSize];
		fread(pBuffer, 1, fileSize, hFile);

		inputJson = json::parse(pBuffer);
	}

	// distribute info
	for(auto serialiser : serialisers)
	{
		serialiser->DeserialiseState(inputJson);
	}
}

void Config::AddStateSerialiser(IConfigSerialisation* serialiser)
{
	serialisers.push_back(serialiser);
}

eErrorCode Config::ParseCommandLine(int32_t argc, char* argv[])
{
	LOGINFO("Config::Parsing command line...");
	
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
			// grab the filename
			loadROMFilename = argv[i+1];
			LOGINFOF("Config::Load filename %s\n", loadROMFilename.c_str());
		}
		if((strcmp(argv[i], "-a") == 0) || (strcmp(argv[i], "--address") == 0))
		{
			char* address = argv[i+1];
			loadROMAddress = strtol(address, NULL, 16);
			LOGINFOF("Config::Address 0x%04x\n", loadROMAddress);	  
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
