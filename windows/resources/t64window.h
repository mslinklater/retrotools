// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "resourcewindowbase.h"

//class ResourceT64;

class T64Window : public ResourceWindowBase
{
public:
	T64Window();
	virtual ~T64Window();
	
	virtual void Draw(void);
	
private:
//	ResourceT64* pResource;
};
