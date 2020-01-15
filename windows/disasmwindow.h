// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "../shared_cpp/windowbase.h"
#include "../config.h"

class Disassembler;
class Cpu6502;
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
	void SetCPU(Cpu6502* pCpu);
	void SetMemory(IMemory* pMemory);
	
	// IStateSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IStateSerialisation
private:
	
	void DrawHeader(void);
	void DrawMainSubWindow(void);
	
	Disassembler* 	pDisasm;
	Cpu6502*		pCpu;
	IMemory*		pMemory;
	
//	bool showTIAHints;
	bool followPC;
};
