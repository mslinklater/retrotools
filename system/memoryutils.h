// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <cstdint>
#include "../errorcodes.h"

class Memory;

class MemoryUtils
{
public:
	static eErrorCode LoadFileToMemory(Memory* pMemory, std::string filename, uint16_t location, uint16_t* bytesRead);
};
