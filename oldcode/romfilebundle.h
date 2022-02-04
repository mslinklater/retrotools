// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#if 0

#pragma once
#include <string>
#include <map>

class IMemory;

class RomFileBundle
{
public:
	RomFileBundle();
	virtual ~RomFileBundle();

	enum ERomType
	{
		kBinary,
		kPrg
	};

	bool Open(std::string filename);
	bool HasLoadAddressBeenSet(){return hasLoadAddressBeenSet;}
	void SetLoadAddress(uint16_t address);
	void CopyToMemory(IMemory* pMemory);
	bool SymbolExists(std::string name);
	uint16_t GetSymbolValue(std::string name);
private:

	ERomType	romType;

	uint8_t* 	pRomFile;
	uint16_t 	loadAddress;
	size_t		romFileSize;
	bool		hasLoadAddressBeenSet;

	// list
//	uint8_t*	pListFile;
//	uint32_t	listFileSize;

	// symbols
	uint8_t*	pSymbolFile;
	size_t		symbolFileSize;
	bool		hasSymbols;

	struct Symbol
	{
		uint16_t	value;
		bool		readOnly;
	};

	std::map<std::string, Symbol> symbolMap;
};

#endif
