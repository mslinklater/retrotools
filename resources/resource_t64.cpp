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
: pTapeRecord(nullptr)
{
}

ResourceT64::~ResourceT64()
{
	if(pData != nullptr)
	{
		delete [] pData;
	}
}

bool ResourceT64::InitFromFilename(const std::string& filename)
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
		LOGINFOF("T64::NumEntries:%d", pTapeRecord->GetNumEntries());
		LOGINFOF("T64::NumUsedEntries:%d", pTapeRecord->GetNumUsedEntries());
		LOGINFOF("T64::UserDescription:%s", pTapeRecord->GetUserDescription().c_str());

		int i=1;
		for(const auto& fileRecord : fileRecords)
		{
			LOGINFOF("T64::FileRecord %d", i++);
			LOGINFOF("T64::Filename %s", fileRecord->GetFilename().c_str());
			LOGINFOF("T64::EntryType %s", fileRecord->GetEntryTypeString().c_str());
			LOGINFOF("T64::StartOffset %08x", fileRecord->GetStartOffset());
			LOGINFOF("T64::StartAddress %04x", fileRecord->GetStartAddress());
			LOGINFOF("T64::EndAddress %04x", fileRecord->GetEndAddress());
		}
	}
	return true;
}

const char* ResourceT64::GetPayload(const std::string& name, std::streamsize& dataSize)
{
	return nullptr;
}
