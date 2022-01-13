// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <iostream>
#include <fstream>

#include "resource_t64.h"
#include "system/log.h"

ResourceT64::ResourceT64()
: pData(nullptr)
, dataSize(0)
, pTapeRecord(nullptr)
{

}

ResourceT64::~ResourceT64()
{
	if(pData != nullptr)
	{
		delete [] pData;
	}
}

void ResourceT64::InitFromFilename(std::string filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if(file)
	{
		dataSize = file.tellg();
		file.seekg(std::ios::beg);
		pData = (char*)new char[dataSize];
		file.read(pData, dataSize);
		file.close();

		// data is read in, now construct content structures
		pTapeRecord = reinterpret_cast<TapeRecord*>(pData);
		int numFiles = pTapeRecord->GetNumUsedEntries();

		for(auto i=0 ; i<numFiles ; i++)
		{
			FileRecord* pFileRecord = reinterpret_cast<FileRecord*>(pData + sizeof(TapeRecord) + i * sizeof(FileRecord));
			fileRecords.push_back(pFileRecord);
		}

		// Now some debug text
		LOGINFOF("T64::Descriptor:%s", pTapeRecord->GetDescriptor().c_str());
		LOGINFOF("T64::Version:%04x", pTapeRecord->GetVersion());
	}
}
