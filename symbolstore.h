// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <map>
#include <vector>
#include "system/errorcodes.h"
#include "symbol.h"
#include "system/istateserialisation.h"

class SymbolStore : public IStateSerialisation
{
public:


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
	void RemoveSymbolAtAddress(uint16_t addr);

	std::vector<Symbol> GetAll();

	ISERIALISATION_API

private:
	std::map<uint16_t,Symbol> symbolMap;
	typedef std::map<uint16_t,Symbol>::iterator SymbolMapIterator;	// make this const ?
};
