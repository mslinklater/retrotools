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

	enum eSymbolType
	{
		kSymbolTypeRead,
		kSymbolTypeWrite,
		kSymbolTypeLabel
	};
	
	struct Symbol
	{
		eSymbolType	type;
		uint16_t 	address;
		std::string name;
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
	
	eErrorCode LoadFromCSV(std::string filename);

	std::vector<Symbol> GetAll();
	
private:
	std::map<uint16_t,std::string> labelMap;
	typedef std::map<uint16_t,std::string>::iterator LabelMapIterator;	// make this const ?
	std::map<uint16_t,std::string> readMap;
	typedef std::map<uint16_t,std::string>::iterator ReadMapIterator;	// make this const ?
	std::map<uint16_t,std::string> writeMap;
	typedef std::map<uint16_t,std::string>::iterator WriteMapIterator;	// make this const ?
};
