// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <iostream>
#include <fstream>
#include <iomanip>
#include "symbolstore.h"
#include "log.h"

SymbolStore::SymbolStore()
{
}

SymbolStore::~SymbolStore()
{
}

bool SymbolStore::HasReadSymbol(uint16_t address)
{
	return readMap.find(address) != readMap.end();
}
std::string SymbolStore::GetReadSymbol(uint16_t address)
{
	return readMap.find(address)->second;
}
bool SymbolStore::HasWriteSymbol(uint16_t address)
{
	return writeMap.find(address) != writeMap.end();
}
std::string SymbolStore::GetWriteSymbol(uint16_t address)
{
	return writeMap.find(address)->second;
}
bool SymbolStore::HasLabelSymbol(uint16_t address)
{
	return labelMap.find(address) != labelMap.end();
}
std::string SymbolStore::GetLabelSymbol(uint16_t address)
{
	return labelMap.find(address)->second;
}
void SymbolStore::AddLabel(uint16_t address, std::string label)
{
	if(!HasLabelSymbol(address))
	{	 
		char buffer[64];
		sprintf(buffer, "label%04x", address);
		labelMap[address] = buffer;
		printf("Label symbol %04x set to %s\n", address, label.c_str());
	}
}

std::vector<SymbolStore::Symbol> SymbolStore::GetAll()
{
	std::vector<Symbol> ret;

	Symbol symbol;
	
	for(LabelMapIterator iter = labelMap.begin() ; iter != labelMap.end() ; iter++)
	{
		symbol.type = kSymbolTypeLabel;
		symbol.address = iter->first;
		symbol.name = iter->second;
		
		ret.push_back(symbol);
	}
	for(ReadMapIterator iter = readMap.begin() ; iter != readMap.end() ; iter++)
	{
		symbol.type = kSymbolTypeRead;
		symbol.address = iter->first;
		symbol.name = iter->second;
		
		ret.push_back(symbol);
	}
	for(WriteMapIterator iter = writeMap.begin() ; iter != writeMap.end() ; iter++)
	{
		symbol.type = kSymbolTypeWrite;
		symbol.address = iter->first;
		symbol.name = iter->second;
		
		ret.push_back(symbol);
	}

	// sort ret
	return ret;
}

eErrorCode SymbolStore::LoadFromCSV(std::string filename)
{
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
			printf("Read symbol %04x set to %s\n", address, readSymbol.c_str());
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
			printf("Write symbol %04x set to %s\n", address, writeSymbol.c_str());
		}
	}
	
	delete [] pBuffer;
	return kError_OK;
}
