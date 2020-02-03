#pragma once
#include <string>
#include <map>

class IMemory;

class RomFileBundle
{
public:
	RomFileBundle();
	virtual ~RomFileBundle();

	void Open(std::string filename);
	void SetLoadAddress(uint16_t address);
	void CopyToMemory(IMemory* pMemory);
private:
	char* 		pRomFile;
	uint16_t 	loadAddress;
	uint32_t	romFileSize;
	bool		hasLoadAddressBeenSet;

	// list

	// symbols
	struct Symbol
	{
		uint16_t	value;
		bool		readOnly;
	};

	std::map<std::string, Symbol> symbolMap;
};