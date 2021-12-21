// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include "../interfaces/imemory.h"

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
	
	// IMemory
	virtual uint8_t 	Read(uint16_t address)
	{
		return pRam[address];
	}
	virtual void 		Write(uint16_t address, uint8_t val)
	{
		pRam[address] = val;
	}
	virtual uint8_t 	DbgRead(uint16_t address){return 0;}
	virtual void 		DbgWrite(uint16_t address, uint8_t val){}
	virtual void		SetHasBeenExecuted(uint16_t address, uint16_t numBytes){}
	// ~IMemory

private:
	uint8_t* pRam;
};
