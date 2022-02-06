// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include "imemory.h"

// NOTE: This is a simple flat 64K address space used to test chips

class Memory64K : public IMemory
{
public:
	Memory64K()
	{
		pRam = new uint8_t[65536];
	}
	virtual ~Memory64K()
	{
		delete [] pRam;
	}

	IMEMORY_API;

private:
	uint8_t* pRam;
};
