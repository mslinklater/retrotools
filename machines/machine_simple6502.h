// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <memory>

#include "machinebase.h"
#include "components/imemory.h"

class MachineSimple6502 : public MachineBase
{
public:
	MachineSimple6502();
	~MachineSimple6502();
private:
	std::shared_ptr<IMemory> pMemory;

	static uint32_t instanceCount;
};
