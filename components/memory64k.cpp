// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "memory64k.h"
#include <assert.h>

uint8_t Memory64K::Read(uint16_t address)
{
	return pRam[address];
}

void Memory64K::Write(uint16_t address, uint8_t val)
{
	pRam[address] = val;
}

uint8_t Memory64K::DbgRead(uint16_t address)
{
	assert(false);
	return 0;
}

void Memory64K::DbgWrite(uint16_t address, uint8_t value)
{
	assert(false);
}

void Memory64K::SetHasBeenExecuted(uint16_t address, uint16_t length)
{
	assert(false);
}
