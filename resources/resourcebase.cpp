// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "resourcebase.h"
#include "system/log.h"

ResourceBase::ResourceBase()
: pData(nullptr)
, dataSize(0)
{

}

ResourceBase::~ResourceBase()
{
	if(pData != nullptr)
	{
		delete [] pData;
		pData = nullptr;
	}
}

#if 0
bool ResourceBase::InitFromFilename(const std::string& filename)
{
	LOGERRORF("ResourceBase::InitFromFilename not implemented for %s", filename.c_str());
	return false;
}
#endif
