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
	const static uint32_t kSymbolFlag_JumpLabel	 	= 1 << 6;
	const static uint32_t kSymbolFlag_BranchLabel 	= 1 << 7;
		
	class Symbol
	{
	public:
		uint32_t	flags;
		uint16_t 	address;
		
//		virtual std::string GetName();
		
		std::string readName;
		std::string writeName;
		std::string labelName;
		
		std::string readDescription;
		std::string writeDescription;
	};
	
	class LabelSymbol : public Symbol
	{
	public:	
	};
	
	SymbolStore();
	virtual ~SymbolStore();

	bool HasReadSymbol(uint16_t address);
	std::string GetReadSymbol(uint16_t address);

	bool HasWriteSymbol(uint16_t address);
	std::string GetWriteSymbol(uint16_t address);
	
	bool HasLabelSymbol(uint16_t address);
	std::string GetLabelSymbol(uint16_t address);
	
	void AddAutoLabel(uint16_t address, std::string label);

	eErrorCode LoadHardwareFromJSON(std::string filename);
	
	void UpdateSymbol(Symbol update);
	
	std::vector<Symbol> GetAll();
	
private:
	std::map<uint16_t,Symbol> symbolMap;
	typedef std::map<uint16_t,Symbol>::iterator SymbolMapIterator;	// make this const ?
};
