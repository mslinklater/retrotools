// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "../system/windowbase.h"

class Disassembler;

class DisassemblyWindow : public WindowBase
{
public:
	
	struct DisasmLine
	{
		
	};
	
	DisassemblyWindow();
	virtual ~DisassemblyWindow();
	
	virtual void Draw(void);
	
	void SetDisassembler(Disassembler* pDisasm);
private:
	
	void DrawHeader(void);
	void DrawMainSubWindow(void);
	
	Disassembler* pDisasm;
	
	bool showTIAHints;
};
