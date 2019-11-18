// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "symbolstore.h"
#include "log.h"
#include "json11/json11.hpp"

SymbolStore::SymbolStore()
{
}

SymbolStore::~SymbolStore()
{
}

bool SymbolStore::HasReadSymbol(uint16_t address)
{
	SymbolMapIterator iter = symbolMap.find(address);
	if(iter == symbolMap.end())
	{
		return false;
	}
	return iter->second.flags & kSymbolFlag_MemoryRead;
}

bool SymbolStore::HasWriteSymbol(uint16_t address)
{
	SymbolMapIterator iter = symbolMap.find(address);
	if(iter == symbolMap.end())
	{
		return false;
	}
	return iter->second.flags & kSymbolFlag_MemoryWrite;
}

bool SymbolStore::HasLabelSymbol(uint16_t address)
{
	SymbolMapIterator iter = symbolMap.find(address);
	if(iter == symbolMap.end())
	{
		return false;
	}
	return iter->second.flags & kSymbolFlag_AddressLabel;
}

std::string SymbolStore::GetReadSymbol(uint16_t address)
{
	return symbolMap.find(address)->second.readName;
}

std::string SymbolStore::GetWriteSymbol(uint16_t address)
{
	return symbolMap.find(address)->second.writeName;
}

std::string SymbolStore::GetLabelSymbol(uint16_t address)
{
	return symbolMap.find(address)->second.labelName;
}

void SymbolStore::UpdateSymbol(SymbolStore::Symbol update)
{
	symbolMap[update.address] = update;
}

void SymbolStore::AddAutoLabel(uint16_t address, std::string label)
{
	SymbolMapIterator iter = symbolMap.find(address);
	
	if(iter == symbolMap.end())
	{
		// new item
		Symbol newSymbol;
		newSymbol.flags |= kSymbolFlag_AddressLabel;
		newSymbol.address = address;
		newSymbol.labelName = label;
		symbolMap[address] = newSymbol;
	}
	else
	{
		iter->second.flags |= kSymbolFlag_MemoryRead;
		iter->second.labelName = label;
	}
}

std::vector<SymbolStore::Symbol> SymbolStore::GetAll()
{
	std::vector<Symbol> ret;

	Symbol symbol;
	
	for(SymbolMapIterator iter = symbolMap.begin() ; iter != symbolMap.end() ; iter++)
	{
		ret.push_back(iter->second);
	}

	return ret;
}

eErrorCode SymbolStore::LoadHardwareFromJSON(std::string filename)
{
	FILE* hFile;
	hFile = fopen(filename.c_str(), "r");
	if(hFile != nullptr)
	{
		fseek(hFile, 0, SEEK_END);
		uint32_t fileSize = ftell(hFile);
		fseek(hFile, 0, SEEK_SET);
		char* readBuffer = new char[fileSize+1];
		memset(readBuffer, 0, fileSize+1);
		fread(readBuffer, fileSize, 1, hFile);
		
		// File leader - now parse

		std::string jsonString = std::string(readBuffer);
		std::string error;
		json11::Json obj = json11::Json::parse(jsonString, error);
		
		if(error.length() > 0)
		{
			LOGERROR(error.c_str());
			return kError_JSONParse;
		}
		
		json11::Json symbolsJSON = obj["symbols"];
		if(symbolsJSON.is_array())
		{
			std::vector<json11::Json> symbolsArray = symbolsJSON.array_items();
			for(auto symbol : symbolsArray)
			{
				Symbol newSymbol;
				uint16_t address = (uint16_t)strtol(symbol["address"].string_value().c_str(), NULL, 16);
				if(symbolMap.find(address) != symbolMap.end())
				{
					newSymbol = symbolMap[address];
				}
				else
				{
					newSymbol.flags = 0;
					newSymbol.address = address;					
				}
				
				if(symbol["label"].is_string())
				{
					newSymbol.labelName = symbol["label"].string_value();
					newSymbol.flags |= kSymbolFlag_AddressLabel;
				}
				if(symbol["read"].is_string())
				{
					newSymbol.readName = symbol["read"].string_value();
					newSymbol.flags |= kSymbolFlag_MemoryRead;
					if(symbol["description"].is_string())
					{
						newSymbol.readDescription = symbol["description"].string_value();
					}
				}
				if(symbol["write"].is_string())
				{
					newSymbol.writeName = symbol["write"].string_value();
					newSymbol.flags |= kSymbolFlag_MemoryWrite;
					if(symbol["description"].is_string())
					{
						newSymbol.writeDescription = symbol["description"].string_value();
					}
				}
				
				newSymbol.flags |= kSymbolFlag_Hardware;
				symbolMap[newSymbol.address] = newSymbol;
			}
		}
		
		// and free up the temp buffer
		delete [] readBuffer;
	}
	return kError_OK;
}


