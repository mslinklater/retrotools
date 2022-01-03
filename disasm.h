// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <cstdint>
#include <map>
#include <vector>

#include "system/errorcodes.h"
#include "components/cpu6502/cpu6502base.h"
#include "system/commandcenter.h"

class IMemory;
class SymbolStore;

class Disassembler : public ICommandHandler
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
		Cpu6502Base::EMnemonic	mnemonic;
		Cpu6502Base::EAddressingMode addressingMode;
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
	void		SetCpu(Cpu6502Base* cpu);
	void		SetSymbolStore(SymbolStore* store);
	
	void 		AddObviousLabels();
	void		DeleteAutoSymbols();

private:
	
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command) override;
	
	void UpdateDetailLines();
	void UpdateLineLabels();
	
	IMemory*			pMemory;
	const Cpu6502Base*	pCpu;
	SymbolStore*		pSymbolStore;
	
	std::vector<Line>	lines;
};
