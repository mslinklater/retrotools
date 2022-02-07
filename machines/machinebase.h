// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class MachineBase
{
public:
	MachineBase();
	virtual ~MachineBase();

	void SysWriteToMemory(unsigned int startAddress, const char* pMemory, unsigned int size);
};
