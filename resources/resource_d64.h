// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "resourcebase.h"

class ResourceD64 : public ResourceBase
{
public:
	ResourceD64();
	virtual ~ResourceD64();

	virtual bool InitFromFilename(const std::string& filename);
private:
};
