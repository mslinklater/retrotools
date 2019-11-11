#pragma once

#include <map>
#include "errorcodes.h"

class SymbolStore
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
	void AddLabel(uint16_t address, std::string label);
	
	eErrorCode LoadFromCSV(std::string filename);
private:
	std::map<uint16_t,std::string> labelMap;
	std::map<uint16_t,std::string> readMap;
	std::map<uint16_t,std::string> writeMap;
};
