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
private:

	ERomType	romType;

	uint8_t* 	pRomFile;
	uint16_t 	loadAddress;
	uint32_t	romFileSize;
	bool		hasLoadAddressBeenSet;

	// list
	uint8_t*	pListFile;
	uint32_t	listFileSize;

	// symbols
	uint8_t*	pSymbolFile;
	uint32_t	symbolFileSize;

	struct Symbol
	{
		uint16_t	value;
		bool		readOnly;
	};

	std::map<std::string, Symbol> symbolMap;
};