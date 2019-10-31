#pragma once

#include <cstdint>
#include <map>

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
	
	Disassembler();
	virtual ~Disassembler();
	
	void 		Init();
	eErrorCode	Disassemble(uint16_t address, uint16_t size, uint16_t org);
	void		DumpToTTY(void);

	void		SetMemory(Memory* mem);
	void		SetCpu(Cpu6502* cpu);
	
private:
	const Memory*					pMemory;
	const Cpu6502*					pCpu;
	std::map<uint16_t,std::string>	readSymbolMap;
	std::map<uint16_t,std::string>	writeSymbolMap;
};
