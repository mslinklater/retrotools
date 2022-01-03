// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "common.h"
#include "stateserialiser.h"

void StateSerialiser::AddStateSerialiser(std::shared_ptr<IStateSerialisation> serialiser)
{
	serialisers.push_back(serialiser);
}

void StateSerialiser::SerialiseAppConfig()
{
	LOGINFO("Config::SerialiseAppConfig");
	json outputJson;

	// grab info

	for(auto serialiser : serialisers)
	{
		serialiser->SerialiseState(outputJson);
	}

	std::string outputString = outputJson.dump();
	FILE* outputFile = fopen("state.json", "w");
	if(outputFile)
	{
		fwrite(outputString.c_str(), outputString.size(), 1, outputFile);
		fclose(outputFile);
	}
}

void StateSerialiser::DeserialiseAppConfig()
{
	LOGINFO("Config::DeserialiseAppConfig");

	json inputJson;

	// Load from file.
	FILE* hFile = fopen("state.json", "r");
	if(hFile != 0)
	{
		fseek(hFile, 0, SEEK_END);
		size_t fileSize = ftell(hFile);
		fseek(hFile, 0, SEEK_SET);
		char* pBuffer = new char[fileSize+1];
		fread(pBuffer, 1, fileSize, hFile);
		pBuffer[fileSize] = 0;

		inputJson = json::parse(pBuffer);

		// distribute info
		for(auto serialiser : serialisers)
		{
			serialiser->DeserialiseState(inputJson);
		}
	}
}

