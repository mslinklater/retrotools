// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <fstream>
#include <iostream>
#include <iomanip>

#include "../log.h"
#include "memoryutils.h"
#include "../components/memory.h"

eErrorCode MemoryUtils::LoadFileToMemory(Memory* pMemory, std::string filename, uint16_t location, uint16_t* bytesRead)
{
	char* pLoadBuffer = 0;
	
	std::ifstream inFile;
	size_t fileSize = 0;
	inFile.open(filename, std::ios::in | std::ios::binary);
	
	if(inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		fileSize = inFile.tellg();
		pLoadBuffer = new char[fileSize];
		inFile.seekg(0, std::ios::beg);
		inFile.read(pLoadBuffer, fileSize);		
        inFile.close();
		*bytesRead = fileSize;

		for(size_t i=0 ; i<fileSize ; i++)
		{
			pMemory->Write(location + i, pLoadBuffer[i]);
		}
	}
	else
	{
		// File load error
		LOGERROR("ERROR - cannot load file");
		return kError_FileNotFound;
	}
	
	if(pLoadBuffer != nullptr)
	{
		delete [] pLoadBuffer;
	}
	
	return kError_OK;
}
