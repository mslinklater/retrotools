// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <cstdint>
#include <map>
#include <vector>

#include "shared_cpp/errorcodes.h"
#include "components/cpu6502.h"
#include "shared_cpp/command.h"

class IMemory;
class Cpu6502;
class SymbolStore;

class Disassembler : public ICommandProcessor
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
	
	void		SetMemory(IMemory* mem);
	void		SetCpu(Cpu6502* cpu);
	void		SetSymbolStore(SymbolStore* store);
	
	void 		AddObviousLabels();
	void		DeleteAutoSymbols();

private:
	
	virtual bool HandleCommand(const Command & command) override;
	
	void UpdateDetailLines();
	void UpdateLineLabels();
	
	IMemory*		pMemory;
	const Cpu6502*		pCpu;
	SymbolStore*	pSymbolStore;
	
	std::vector<Line>	lines;
};
