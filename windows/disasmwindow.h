// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "windowbase.h"
#include "system/istateserialisation.h"

class Disassembler;
class Cpu6502Base;
class IMemory;

class DisassemblyWindow : public WindowBase, public IStateSerialisation
{
public:
	
	struct DisasmLine
	{
		
	};
	
	DisassemblyWindow();
	virtual ~DisassemblyWindow();
	
	virtual void Draw(void);
	
	void SetDisassembler(Disassembler* pDisasm);
	void SetCPU(Cpu6502Base* pCpu);
	void SetMemory(IMemory* pMemory);

	ISERIALISATION_API
private:
	
	void DrawHeader(void);
	void DrawMainSubWindow(void);
	
	Disassembler* 	pDisasm;
	Cpu6502Base*	pCpu;
	IMemory*		pMemory;
	
//	bool showTIAHints;
	bool followPC;
};
