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

		// Master tape record
		pTapeRecord = reinterpret_cast<TapeRecord*>(pData);
		int numFiles = pTapeRecord->GetNumUsedEntries();

		// Now the file records themselves
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

		// Now set up the payloads
		for(const auto& fileRecord : fileRecords)
		{
			ResourcePayload payload;
			payload.name = fileRecord->GetFilename();
			payload.offset = fileRecord->GetStartOffset();
			payload.size = fileRecord->GetEndAddress() - fileRecord->GetStartAddress() + 1;
			payloads.push_back(payload);
		}


	}
	return true;
}
