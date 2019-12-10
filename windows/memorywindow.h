// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "../system/windowbase.h"
#include "../config.h"

class Memory;

class MemoryWindow : public WindowBase, public IConfigSerialisation
{
public:
	
	struct Line {	// TODO: Move this out in to a view or helper class
		char	address[5];		// memory address of line
		char	value[16][3];	// the 16 memory location values
		char	key[16];		// representative figure for each byte
	};
	
	MemoryWindow();
	virtual ~MemoryWindow();

	void SetMemory(Memory* mem);
	
	virtual void Draw(void);

	// IConfigSerialisation
	void SerialiseState(json& object);
	void DeserialiseState(json& object);
	// ~IConfigSerialisation

private:
	void DrawLine(uint16_t startAddress);
	
	Memory*		pMemory;

	bool showReadWrite;
};
