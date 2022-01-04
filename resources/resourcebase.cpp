// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "resourcebase.h"
#include "system/log.h"

ResourceBase::ResourceBase()
{

}

ResourceBase::~ResourceBase()
{

}

void ResourceBase::InitFromFilename(std::string filename)
{
	LOGERRORF("ResourceBase::InitFromFilename not implemented for %s", filename.c_str());
}
