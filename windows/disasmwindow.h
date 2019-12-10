// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "../system/windowbase.h"
#include "../config.h"

class Disassembler;
class Cpu6502;
class Memory;

class DisassemblyWindow : public WindowBase, public IConfigSerialisation
{
public:
	
	struct DisasmLine
	{
		
	};
	
	DisassemblyWindow();
	virtual ~DisassemblyWindow();
	
	virtual void Draw(void);
	
	void SetDisassembler(Disassembler* pDisasm);
	void SetCPU(Cpu6502* pCpu);
	void SetMemory(Memory* pMemory);
	
	// IConfigSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IConfigSerialisation
private:
	
	void DrawHeader(void);
	void DrawMainSubWindow(void);
	
	Disassembler* 	pDisasm;
	Cpu6502*		pCpu;
	Memory*			pMemory;
	
	bool showTIAHints;
	bool followPC;
};
