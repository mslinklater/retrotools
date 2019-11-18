#pragma once

#include <string>
#include <stdint.h>
	
class Symbol
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

	uint32_t	flags;
	uint16_t 	address;
		
	std::string readName;
	std::string writeName;
	std::string labelName;
		
	std::string readDescription;
	std::string writeDescription;
};

