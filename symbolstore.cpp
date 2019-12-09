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
#include "json.hpp"
using json = nlohmann::json;

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
	return iter->second.flags & Symbol::kSymbolFlag_MemoryRead;
}

bool SymbolStore::HasWriteSymbol(uint16_t address)
{
	SymbolMapIterator iter = symbolMap.find(address);
	if(iter == symbolMap.end())
	{
		return false;
	}
	return iter->second.flags & Symbol::kSymbolFlag_MemoryWrite;
}

bool SymbolStore::HasLabelSymbol(uint16_t address)
{
	SymbolMapIterator iter = symbolMap.find(address);
	if(iter == symbolMap.end())
	{
		return false;
	}
	return iter->second.flags & Symbol::kSymbolFlag_AddressLabel;
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

void SymbolStore::UpdateSymbol(Symbol update)
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
		newSymbol.flags |= Symbol::kSymbolFlag_AddressLabel;
		newSymbol.flags |= Symbol::kSymbolFlag_Auto;
		newSymbol.address = address;
		newSymbol.labelName = label;
		symbolMap[address] = newSymbol;
	}
}

void SymbolStore::RemoveSymbolAtAddress(uint16_t addr)
{
	if(symbolMap.find(addr) != symbolMap.end())
	{
		symbolMap.erase(addr);
	}
}

std::vector<Symbol> SymbolStore::GetAll()
{
	std::vector<Symbol> ret;

	Symbol symbol;
	
	for(SymbolMapIterator iter = symbolMap.begin() ; iter != symbolMap.end() ; iter++)
	{
		ret.push_back(iter->second);
	}

	return ret;
}

void SymbolStore::SerialiseState(json& object)
{
	json symbolStoreJson = json::object();

	json symbolsArrayJson = json::array();

	for(auto symbol : symbolMap)
	{
		json symbolJson = json::object();
		if(symbol.second.Serialise(symbolJson))
		{
			symbolsArrayJson.push_back(symbolJson);
		}
	}

	symbolStoreJson["symbols"] = symbolsArrayJson;
	object["symbolstore"] = symbolStoreJson;
}

void SymbolStore::DeserialiseState(json& object)
{
	json symbolStoreJson = object["symbolstore"];
	if(symbolStoreJson.is_object())
	{
		json symbolsArrayJson = symbolStoreJson["symbols"];
		if(symbolsArrayJson.is_array())
		{
			for(auto symbolJson : symbolsArrayJson)
			{
				Symbol symbol;
				symbol.Deserialise(symbolJson);
				symbolMap[symbol.address] = symbol;
			}
		}
	}
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
		json inputJson = json::parse(jsonString);

		if(error.length() > 0)
		{
			LOGERROR(error.c_str());
			return kError_JSONParse;
		}

		json symbolsJSON = inputJson["symbols"];
		if(symbolsJSON.is_array())
		{
			for(auto symbol : symbolsJSON)
			{
				Symbol newSymbol;
				std::string addressString = symbol["address"].get<std::string>();
				uint16_t address = (uint16_t)strtol(addressString.c_str(), NULL, 16);
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
					newSymbol.labelName = symbol["label"].get<std::string>();
					newSymbol.flags |= Symbol::kSymbolFlag_AddressLabel;
				}
				if(symbol["read"].is_string())
				{
					newSymbol.readName = symbol["read"].get<std::string>();
					newSymbol.flags |= Symbol::kSymbolFlag_MemoryRead;
					if(symbol["description"].is_string())
					{
						newSymbol.readDescription = symbol["description"].get<std::string>();
					}
				}
				if(symbol["write"].is_string())
				{
					newSymbol.writeName = symbol["write"].get<std::string>();
					newSymbol.flags |= Symbol::kSymbolFlag_MemoryWrite;
					if(symbol["description"].is_string())
					{
						newSymbol.writeDescription = symbol["description"].get<std::string>();
					}
				}

				newSymbol.flags |= Symbol::kSymbolFlag_Hardware;
				symbolMap[newSymbol.address] = newSymbol;
			}
		}
		// and free up the temp buffer
		delete [] readBuffer;
	}
	return kError_OK;
}


