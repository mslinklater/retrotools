// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <map>
#include <vector>
#include "errorcodes.h"

class SymbolStore
{
public:

	const static uint32_t kSymbolFlag_MemoryRead 	= 1 << 0;
	const static uint32_t kSymbolFlag_MemoryWrite 	= 1 << 1;
	const static uint32_t kSymbolFlag_AddressLabel 	= 1 << 2;
	const static uint32_t kSymbolFlag_Hardware	 	= 1 << 3;
	const static uint32_t kSymbolFlag_User		 	= 1 << 4;
	const static uint32_t kSymbolFlag_Auto		 	= 1 << 5;
	
	struct Symbol
	{
		uint32_t	flags;
		uint16_t 	address;
		std::string readName;
		std::string writeName;
		std::string labelName;
	};
	
	SymbolStore();
	virtual ~SymbolStore();

	bool HasReadSymbol(uint16_t address);
	std::string GetReadSymbol(uint16_t address);

	bool HasWriteSymbol(uint16_t address);
	std::string GetWriteSymbol(uint16_t address);
	
	bool HasLabelSymbol(uint16_t address);
	std::string GetLabelSymbol(uint16_t address);
	
	void AddLabel(uint16_t address, std::string label);

	eErrorCode LoadHardwareFromJSON(std::string filename);
	
	eErrorCode LoadFromCSV(std::string filename);

	std::vector<Symbol> GetAll();
	
private:
	std::map<uint16_t,Symbol> symbolMap;
	typedef std::map<uint16_t,Symbol>::iterator SymbolMapIterator;	// make this const ?

	// remove these
//	std::map<uint16_t,std::string> labelMap;
//	typedef std::map<uint16_t,std::string>::iterator LabelMapIterator;	// make this const ?
//	std::map<uint16_t,std::string> readMap;
//	typedef std::map<uint16_t,std::string>::iterator ReadMapIterator;	// make this const ?
//	std::map<uint16_t,std::string> writeMap;
//	typedef std::map<uint16_t,std::string>::iterator WriteMapIterator;	// make this const ?
	
};
