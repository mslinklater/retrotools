#include "romfilebundle.h"

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

void RomFileBundle::Open(string filename)
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
		std::string suffix = filename.substr(dotPos+1);
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

	std::ifstream romFile;
	size_t romFileSize;
	romFile.open(filename, std::ios::in | std::ios::binary);
	
	if(romFile.is_open())
	{

	}
}

void RomFileBundle::SetLoadAddress(uint16_t addr)
{
	loadAddress = addr;
	hasLoadAddressBeenSet = true;
}

