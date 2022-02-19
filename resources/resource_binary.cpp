// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <iostream>
#include <fstream>

#include "resource_binary.h"
#include "system/log.h"

ResourceBinary::ResourceBinary()
{
}

ResourceBinary::~ResourceBinary()
{
}

bool ResourceBinary::InitFromFilename(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if(file)
	{
		dataSize = file.tellg();
		file.seekg(std::ios::beg);
		pData = (char*)new char[dataSize];
		file.read(pData, dataSize);
		file.close();
	}
	return true;
}
