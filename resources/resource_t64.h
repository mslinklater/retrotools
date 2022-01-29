// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include "resourcebase.h"

class ResourceT64 : public ResourceBase
{
public:
	ResourceT64();
	virtual ~ResourceT64();

	struct TapeRecord
	{
		char descriptor[32];
		char version[2];
		char numEntries[2];
		char numUsedEntries[2];
		char free[2];
		char userDescription[24];

		std::string GetDescriptor() const
		{
			return std::string(&descriptor[0]);
		}
		unsigned short GetVersion()
		{
			return version[0] | (version[1] << 8);
		}
		unsigned short GetNumEntries()
		{
			return numEntries[0] | (numEntries[1] << 8);
		}
		unsigned short GetNumUsedEntries()
		{
			return numUsedEntries[0] | (numUsedEntries[1] << 8);
		}
		std::string GetUserDescription()
		{
			return std::string(&userDescription[0]);
		}
	};

	enum class EFileRecordEntryType
	{
		FreeEntry = 0,
		NormalTapeFile,
		TapeFileWithHeader,
		MemorySnapshot,
		TapeBlock,
		DigitizedStream
	};

	struct FileRecord
	{
		char entryType;
		char normalTapeFile;
		char startAddress[2];
		char endAddress[2];
		char free[2];
		char startOffset[4];
		char free2[4];
		char filename[16];

		std::string GetEntryTypeString()
		{
			switch(entryType)
			{
				case (char)EFileRecordEntryType::FreeEntry: return "FreeEntry";
				case (char)EFileRecordEntryType::NormalTapeFile: return "NormalTapeFile";
				case (char)EFileRecordEntryType::TapeFileWithHeader: return "TapeFileWithHeader";
				case (char)EFileRecordEntryType::MemorySnapshot: return "MemorySnapshot";
				case (char)EFileRecordEntryType::TapeBlock: return "TapeBlock";
				case (char)EFileRecordEntryType::DigitizedStream: return "DigitizedStream";
			}
			return "Unknown";
		}
		EFileRecordEntryType GetEntryType()
		{
			return(EFileRecordEntryType)entryType;
		}
		unsigned short GetStartAddress(){return startAddress[0] | (startAddress[1] << 8);}
		unsigned short GetEndAddress(){return endAddress[0] | (endAddress[1] << 8);}
		unsigned int GetStartOffset(){return startOffset[0] | (startOffset[1] << 8) | (startOffset[2] << 16) | (startOffset[3] << 24);}
		std::string GetFilename(){return std::string(&filename[0]);}
	};

	virtual bool InitFromFilename(const std::string& filename);

	const TapeRecord *GetTapeRecord() const { return pTapeRecord; }

private:
	char* 	pData;
	std::streamsize	dataSize;
	TapeRecord* pTapeRecord;
	std::vector<FileRecord*> fileRecords;
};
