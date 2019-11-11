#pragma once

#include <cstdint>
#include <map>
#include <vector>

#include "errorcodes.h"
#include "components/cpu6502.h"

class Memory;
class Cpu6502;
class SymbolStore;

class Disassembler
{
public:
	
	enum SymbolType
	{
		kRead,
		kWrite,
		kReadWrite
	};

	static const uint32_t kFlagSymbol = 1 << 0;
	static const uint32_t kFlagLabel = 	1 << 1;
	
	struct Line
	{
		bool used;
		Cpu6502::eMnemonic	mnemonic;
		Cpu6502::eAddressingMode addressingMode;
		Cpu6502::eMemoryOp memoryOp;
		uint8_t		opcode;
		uint8_t		param1;
		uint8_t		param2;
		uint16_t	address;
		std::string label;		// max size plus null
		std::string addressString;	// '0x0000' plus null
		std::string bytes;		// '00 00 00' plus null
		std::string mnemonicString;	// max size plus null
		std::string detail;
		std::vector<std::string>	hints;
		uint32_t	flags;		// contenx of the instruction - to help visualisation
	};

	Disassembler();
	virtual ~Disassembler();
	
	void 		Init();
	eErrorCode	Disassemble(uint16_t address, uint16_t size, uint16_t org);

	int			GetNumLines();
	const Line& GetLine(int num);
	
	void		SetMemory(Memory* mem);
	void		SetCpu(Cpu6502* cpu);
	void		SetSymbolStore(SymbolStore* store);
	
private:
	
	void AddObviousLabels();
	void UpdateDetailLines();
	void UpdateLineLabels();
	
	const Memory*		pMemory;
	const Cpu6502*		pCpu;
	SymbolStore*	pSymbolStore;
	
	std::vector<Line>	lines;
	uint32_t			anonLabelCount;
};
