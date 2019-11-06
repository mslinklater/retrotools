#pragma once

#include <cstdint>
#include <map>
#include <vector>

#include "errorcodes.h"

class Memory;
class Cpu6502;

class Disassembler
{
public:
	
	enum SymbolType
	{
		kRead,
		kWrite,
		kReadWrite
	};
	
	struct Line
	{
		bool used;
		std::string label;		// max size plus null
		std::string address;	// '0x0000' plus null
		std::string bytes;		// '00 00 00' plus null
		std::string mnemonic;	// max size plus null
		std::string detail;		
	};

	Disassembler();
	virtual ~Disassembler();
	
	void 		Init();
	eErrorCode	Disassemble(uint16_t address, uint16_t size, uint16_t org);

	int			GetNumLines();
	const Line& GetLine(int num);
	
	void		SetMemory(Memory* mem);
	void		SetCpu(Cpu6502* cpu);
	
private:
	const Memory*		pMemory;
	const Cpu6502*		pCpu;
	
	std::vector<Line>	lines;
};
