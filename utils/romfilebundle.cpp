#include "romfilebundle.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#include "../shared_cpp/log.h"
#include "../interfaces/imemory.h"

using namespace std;

RomFileBundle::RomFileBundle()
: pRomFile(nullptr)
, loadAddress(0)
, hasLoadAddressBeenSet(false)
{

}

RomFileBundle::~RomFileBundle()
{
	if(pRomFile != nullptr)
	{
		delete pRomFile;
		pRomFile = nullptr;
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
	size_t romFileSize;
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
	// is there a list file to go with it ?
	return true;
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
				pMemory->DbgWrite(loadAddress + i, pRomFile[i]);
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

