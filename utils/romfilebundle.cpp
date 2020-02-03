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
	}
	else
	{
	}
	
}

void RomFileBundle::SetLoadAddress(uint16_t addr)
{
	loadAddress = addr;
	hasLoadAddressBeenSet = true;
}

