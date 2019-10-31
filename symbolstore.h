#pragma once

#include <map>
#include "errorcodes.h"

class SymbolStore
{
public:
	SymbolStore();
	virtual ~SymbolStore();
	
	eErrorCode LoadFromCSV(std::string filename);
private:
	std::map<uint16_t,std::string> readMap;
	std::map<uint16_t,std::string> writeMap;
};
