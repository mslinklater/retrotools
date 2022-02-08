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
		unsigned short GetVersion() const
		{
			return version[0] | (version[1] << 8);
		}
		unsigned short GetNumEntries() const
		{
			return numEntries[0] | (numEntries[1] << 8);
		}
		unsigned short GetNumUsedEntries() const
		{
			return numUsedEntries[0] | (numUsedEntries[1] << 8);
		}
		std::string GetUserDescription() const
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

		std::string GetEntryTypeString() const
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
		EFileRecordEntryType GetEntryType() const
		{
			return(EFileRecordEntryType)entryType;
		}
		unsigned short GetStartAddress() const {return startAddress[0] | (startAddress[1] << 8);}
		unsigned short GetEndAddress() const {return endAddress[0] | (endAddress[1] << 8);}
		unsigned int GetStartOffset() const {return startOffset[0] | (startOffset[1] << 8) | (startOffset[2] << 16) | (startOffset[3] << 24);}
		std::string GetFilename() const {return std::string(&filename[0]);}
		char GetByte(unsigned short addr) const { return 0; }
	};

//	virtual bool InitFromFilename(const std::string& filename);
//	virtual const char* GetPayload(const std::string& name, std::streamsize& dataSize);

	IRESOURCEBASE_API;

	const TapeRecord *GetTapeRecord() const { return pTapeRecord; }
	int GetNumFileRecords() const { return fileRecords.size(); }
	const FileRecord *GetFileRecord(int num) const { return fileRecords[num]; }

private:
	TapeRecord* pTapeRecord;
	std::vector<FileRecord*> fileRecords;
};
