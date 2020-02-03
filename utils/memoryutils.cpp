// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <fstream>
#include <iostream>
#include <iomanip>

#include "../shared_cpp/log.h"
#include "memoryutils.h"
#include "../interfaces/imemory.h"

eErrorCode MemoryUtils::LoadFileToMemory(IMemory* pMemory, std::string filename, uint16_t location, uint16_t* bytesRead)
{
	// work out what type of file it is
	
	eFileType fileType = kBinary;
		
	uint8_t* pLoadBuffer = 0;
	
	std::ifstream inFile;
	size_t fileSize = 0;
	inFile.open(filename, std::ios::in | std::ios::binary);
	
	if(inFile.is_open())
	{
		// Now we know the file exists lets try to work out what format it is
		if(filename.find(".bin") == filename.size() - 4)
		{
			LOGINFO("MemoryUtils::Found binary file");
			fileType = kBinary;
		}
		else if(filename.find(".prg") == filename.size() - 4)
		{
			LOGINFO("MemoryUtils::Found PRG file");
			fileType = kPrg;
		}

		// Now load the fecker
		inFile.seekg(0, std::ios::end);
		fileSize = inFile.tellg();
		pLoadBuffer = new uint8_t[fileSize];
		inFile.seekg(0, std::ios::beg);
		inFile.read((char*)pLoadBuffer, fileSize);		
        inFile.close();
		*bytesRead = fileSize;

		// file now read in...
		
		switch(fileType)
		{
			case kBinary:
				LOGINFOF("MemoryUtils::Loading file %s to 0x%04x - size 0x%04x", filename.c_str(), location, fileSize);
				// straight copy to memory
				for(size_t i=0 ; i<fileSize ; i++)
				{
					pMemory->DbgWrite(location + i, pLoadBuffer[i]);
				}
				break;
			case kPrg:
				location = (((uint16_t)pLoadBuffer[1]) << 8) | pLoadBuffer[0];
				LOGINFOF("MemoryUtils::Loading file %s to 0x%04x - size 0x%04x", filename.c_str(), location, fileSize-2);
				for(size_t i=0 ; i<fileSize-2 ; i++)
				{
					pMemory->DbgWrite(location + i, pLoadBuffer[i+2]);
				}
				break;
		}
	}
	else
	{
		// File load error
		LOGERRORF("ERROR - file not found (%s)", filename.c_str());
		return kError_FileNotFound;
	}
	
	if(pLoadBuffer != nullptr)
	{
		delete [] pLoadBuffer;
	}
	
	return kError_OK;
}
