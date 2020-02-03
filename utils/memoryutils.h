// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <cstdint>
#include "../shared_cpp/errorcodes.h"

class IMemory;

class MemoryUtils
{
public:
	
	enum eFileType
	{
		kBinary,
		kPrg
	};
	
	static eErrorCode LoadFileToMemory(IMemory* pMemory, std::string filename, uint16_t location, uint16_t* bytesRead);
};
