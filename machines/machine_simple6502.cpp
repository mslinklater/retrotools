// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "machine_simple6502.h"
#include "system/common.h"
#include "components/memory64k.h"

MachineSimple6502::MachineSimple6502()
{
	LOGINFO("MachineSimple6502::MachineSimple6502");

	pMemory = std::make_shared<Memory64K>();
}

MachineSimple6502::~MachineSimple6502()
{
	LOGINFO("MachineSimple6502::~MachineSimple6502");
}

