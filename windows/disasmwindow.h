// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class Disassembler;

class DisassemblyWindow
{
public:
	
	struct DisasmLine
	{
		
	};
	
	DisassemblyWindow();
	virtual ~DisassemblyWindow();
	
	void Draw(bool* pShow);
	void SetDisassembler(Disassembler* pDisasm);
private:
	
	void DrawHeader(void);
	void DrawMainSubWindow(void);
	
	Disassembler* pDisasm;
	
	bool showTIAHints;
};
