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
	Disassembler* pDisasm;
};
