// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <iostream>
#include <fstream>
#include <iomanip>
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

void SymbolStore::AddLabel(uint16_t address, std::string label)
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
		char* readBuffer = new char[fileSize];
		fread(readBuffer, fileSize, 1, hFile);
		delete [] readBuffer;
	}
	return kError_OK;
}


eErrorCode SymbolStore::LoadFromCSV(std::string filename)
{
	/*
	std::ifstream inFile;
	size_t size = 0;
	inFile.open(filename, std::ios::in);
	
	char* pBuffer;
	
	if(inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		size = inFile.tellg();
		pBuffer = new char[size];
		inFile.seekg(0, std::ios::beg);
		inFile.read(pBuffer, size);		
        inFile.close();
	}
	else
	{
		// File load error
		LOGERROR("ERROR - cannot load file");
		return kError_FileNotFound;
	}

	// loaded, now parse the CSV
	uint32_t readPtr = 0;
	// miss first line
	while(pBuffer[readPtr++] != '\n')
	{
	}
	while(readPtr < size-1)
	{
		// process line
		uint32_t startLine = readPtr;
		// find first comma
		while(pBuffer[readPtr] != ',')
		{
			readPtr++;
		}
		std::string addressString(pBuffer+startLine, readPtr-startLine);
		uint16_t address = strtol(addressString.c_str(), NULL, 16);
		readPtr++;
		// read value
		uint16_t startReadSymbol = readPtr;
		// find second comma
		while(pBuffer[readPtr] != ',')
		{
			readPtr++;
		}
		if(readPtr != startReadSymbol)
		{
			std::string readSymbol(pBuffer+startReadSymbol, readPtr-startReadSymbol);
			readMap[address] = readSymbol;
			LOGINFOF("Read symbol %04x set to %s\n", address, readSymbol.c_str());
		}
		readPtr++;
		uint16_t startWriteSymbol = readPtr;
		// find newline
		while(pBuffer[readPtr] != '\n')
		{
			readPtr++;
		}
		if(readPtr != startWriteSymbol)
		{
			std::string writeSymbol(pBuffer+startWriteSymbol, readPtr-startWriteSymbol);
			writeMap[address] = writeSymbol;
			LOGINFOF("Write symbol %04x set to %s\n", address, writeSymbol.c_str());
		}
	}
	
	delete [] pBuffer;
	*/
	return kError_OK;
}
