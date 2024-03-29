// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "system/window/windowbase.h"
#include "system/istateserialisation.h"

class Memory2600;

class Memory2600Window : public WindowBase, public IStateSerialisation
{
public:
	
	struct Line {	// TODO: Move this out in to a view or helper class
		char	address[5];		// memory address of line
		char	value[16][3];	// the 16 memory location values
		char	key[16];		// representative figure for each byte
	};
	
	Memory2600Window();
	virtual ~Memory2600Window();

	void SetMemory(Memory2600* mem);
	
	virtual void Draw(void) override;

	ISERIALISATION_API

private:
	void DrawLine(uint16_t startAddress);
	
	Memory2600*		pMemory;

	bool showReadWrite;
};
