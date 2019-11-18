#pragma once

#include <string>
#include <stdint.h>

class Symbol
{
public:
	uint32_t	flags;
	uint16_t 	address;
		
	std::string readName;
	std::string writeName;
	std::string labelName;
		
	std::string readDescription;
	std::string writeDescription;
};

