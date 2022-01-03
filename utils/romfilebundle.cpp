// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "romfilebundle.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#include "common.h"
#include "../interfaces/imemory.h"

using namespace std;

RomFileBundle::RomFileBundle()
: pRomFile(nullptr)
, loadAddress(0)
, romFileSize(0)
, hasLoadAddressBeenSet(false)
//, pListFile(nullptr)
//, listFileSize(0)
, pSymbolFile(nullptr)
, symbolFileSize(0)
, hasSymbols(false)
{

}

RomFileBundle::~RomFileBundle()
{
	if(pRomFile != nullptr)
	{
		delete pRomFile;
		pRomFile = nullptr;
	}
	if(pSymbolFile != nullptr)
	{
		delete pSymbolFile;
		pSymbolFile = nullptr;
	}
}

bool RomFileBundle::Open(string filename)
{
	// is there a filetype ?
	size_t dotPos = filename.find_last_of('.');

	if(dotPos == string::npos)
	{
		// no suffix so presuming a raw binary file
		romType = ERomType::kBinary;
	}
	else
	{
		string suffix = filename.substr(dotPos+1);
		if(suffix == "prg")
		{
			romType = ERomType::kPrg;
		}
		else
		{
			romType = ERomType::kBinary;
		}
	}	
	// load the rom file

	ifstream romFile;
	romFile.open(filename, ios::in | ios::binary);
	
	if(romFile.is_open())
	{
		romFile.seekg(0, ios::end);
		romFileSize = romFile.tellg();
		pRomFile = new uint8_t[romFileSize];
		romFile.seekg(0, ios::beg);
		romFile.read((char*)pRomFile, romFileSize);
		romFile.close();

		// If this is a PRG we can grab the load address from the first 2 bytes
		if(romType == ERomType::kPrg)
		{
			loadAddress = pRomFile[0] | (pRomFile[1] << 8);
			hasLoadAddressBeenSet = true;
		}
	}
	else
	{
		return false;	// File not found
	}

	// is there a symbol file to go with it ?
	string filenameRoot = filename.substr(0, dotPos);
	ifstream symFile;
	symFile.open(filenameRoot + ".sym", ios::in);
	
	if(symFile.is_open())
	{
		symFile.seekg(0, ios::end);
		symbolFileSize = symFile.tellg();
		pSymbolFile = new uint8_t[symbolFileSize];
		symFile.seekg(0, ios::beg);
		symFile.read((char*)pSymbolFile, symbolFileSize);
		symFile.close();

		hasSymbols = true;

		// iterate through each line of the file
		uint32_t lineStart = 0;
		for(uint32_t ptr = 0 ; ptr < symbolFileSize ; ptr++)
		{
			if(pSymbolFile[ptr] == 0x0a)
			{
				string line((const char*)&(pSymbolFile[lineStart]), (int)(ptr-lineStart));

				if(line[0] != '-')
				{
					// valid line
					size_t endSymbol = line.find_first_of(" ");
					size_t startValue = line.find_first_not_of(" ", endSymbol);
					size_t endValue = line.find_first_of(" ", startValue);

					string symbol = line.substr(0,endSymbol);
					string valueString = line.substr(startValue, endValue);
					uint16_t value = stoi(valueString, 0, 16);

					Symbol newSymbol;
					newSymbol.value = value;
					newSymbol.readOnly = false;
					symbolMap[symbol] = newSymbol;
				}

				lineStart = ptr+1;
			}
		}
	}

	// is there a list file to go with it ?
	return true;
}

bool RomFileBundle::SymbolExists(std::string name)
{
	return symbolMap.find(name) != symbolMap.end();
}

uint16_t RomFileBundle::GetSymbolValue(std::string name)
{
	if(!SymbolExists(name))
	{
		return 0xffff;
	}
	return symbolMap[name].value;
}

void RomFileBundle::CopyToMemory(IMemory* pMemory)
{
	switch(romType)
	{
		case ERomType::kBinary:
			break;
		case ERomType::kPrg:
			for(uint32_t i=0 ; i<romFileSize-2 ; i++)
			{
				pMemory->Write(loadAddress + i, pRomFile[i+2]);
			}
			break;
	}
}

void RomFileBundle::SetLoadAddress(uint16_t addr)
{
	if(hasLoadAddressBeenSet)
	{
		LOGERROR("RomFileBundle::ROM Load address is already set.");
	}
	loadAddress = addr;
	hasLoadAddressBeenSet = true;
}

