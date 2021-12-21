// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <string>
#include <sstream>
#include <cassert>
#include "disasm.h"
#include "common.h"
#include "symbolstore.h"
#include "components/cpu6502/cpu6502alpha.h"
#include "interfaces/imemory.h"

//static const uint32_t kMaxDisasmLines = 4096;

Disassembler::Disassembler()
{
	CommandCenter::Instance()->Subscribe("refreshDisassembly", this);
}

Disassembler::~Disassembler()
{
}

void Disassembler::Init(void)
{
	LOGINFO("Disassembler::Init()\n");

}

void Disassembler::SetMemory(IMemory* mem)
{
	pMemory = mem;
}

void Disassembler::SetCpu(Cpu6502Base* cpu)
{
	pCpu = cpu;
}

void Disassembler::SetSymbolStore(SymbolStore* store)
{
	pSymbolStore = store;
}

eErrorCode Disassembler::Disassemble(uint16_t address, uint16_t size, uint16_t org)
{
	assert(pCpu != nullptr);
	assert(pMemory != nullptr);
	
	// get rid of the old disassembly
	
	lines.clear();
	
	uint32_t currentAddress = address;
	while(currentAddress < (uint32_t)address + (uint32_t)size)
	{
		char buffer[128];
		Line thisLine;
		thisLine.used = true;
		thisLine.flags = 0;
		thisLine.hints.clear();
		
		// address
		sprintf(buffer, "%04x", currentAddress);
		thisLine.addressString = buffer;
		thisLine.address = currentAddress;
		
		uint8_t opcode = pMemory->DbgRead(currentAddress);
		const Cpu6502Base::Opcode* opcodeInfo = pCpu->GetOpcode(opcode);

		if(opcodeInfo->valid)
		{
			// bytes
			thisLine.param1 = pMemory->DbgRead(currentAddress+1);
			thisLine.param2 = pMemory->DbgRead(currentAddress+2);
			
			switch(opcodeInfo->length)
			{
				case 3:
					sprintf(buffer, "%02x %02x %02x", opcode, thisLine.param1, thisLine.param2);
					break;
				case 2:
					sprintf(buffer, "%02x %02x   ", opcode, thisLine.param1);
					break;
				default:
					sprintf(buffer, "%02x      ", opcode);
					break;
			}
			thisLine.bytes = buffer;

			// opcode
			Cpu6502Base::EMnemonic mnemonic = pCpu->GetOpcode(opcode)->mnemonic;
			sprintf(buffer, "%s", pCpu->GetMnemonicString(mnemonic).c_str());
			thisLine.mnemonicString = buffer;
			thisLine.mnemonic = mnemonic;
			thisLine.addressingMode = pCpu->GetOpcode(opcode)->addrMode;
			//thisLine.memoryOp = pCpu->GetOpcode(opcode)->memoryOp;
			// Move to the next instruction
			currentAddress+=opcodeInfo->length;
		}
		else
		{
			thisLine.mnemonicString = "INVALID";
			currentAddress++;	  
		}
		lines.push_back(thisLine);
	}

	UpdateLineLabels();
	UpdateDetailLines();

	return kError_OK;
}

void Disassembler::DeleteAutoSymbols()
{
	std::vector<Symbol> symbols = pSymbolStore->GetAll();
	for(auto symbol : symbols)
	{
		if(symbol.flags & Symbol::kSymbolFlag_Auto)
		{
			pSymbolStore->RemoveSymbolAtAddress(symbol.address);
		}
	}
	UpdateLineLabels();
	UpdateDetailLines();
}

void Disassembler::UpdateDetailLines()
{
	char buffer[128];
	uint16_t addr;
	for(uint32_t iLine=0 ; iLine<lines.size() ; iLine++)
	{
		Line* pLine = &lines[iLine];
		sprintf(buffer, " ");
		switch(pLine->addressingMode)
		{
			case Cpu6502Base::EAddressingMode::kAddrModeAbsolute:
//				if(pLine->memoryOp == Cpu6502::EMemoryOp::kNone)
				{
					uint16_t value = pLine->param2 << 8 | pLine->param1;
					if(pSymbolStore->HasLabelSymbol(value))
					{
						sprintf(buffer, "%s", pSymbolStore->GetLabelSymbol(value).c_str());
						pLine->flags |= kFlagLabel;
					}
					else
					{	
						sprintf(buffer, "$%02x%02x", pLine->param2, pLine->param1);
					}
				}
#if 0
				else if(pLine->memoryOp == Cpu6502::EMemoryOp::kRead)
				{
					uint16_t value = pLine->param2 << 8 | pLine->param1;
					if(pSymbolStore->HasReadSymbol(value))
					{
						sprintf(buffer, "%s", pSymbolStore->GetReadSymbol(value).c_str());
						pLine->flags |= kFlagSymbol;
					}
					else
					{	
						sprintf(buffer, "$%02x%02x", pLine->param2, pLine->param1);
					}
				}
				else if(pLine->memoryOp == Cpu6502::EMemoryOp::kWrite)
				{
					uint16_t value = pLine->param2 << 8 | pLine->param1;
					if(pSymbolStore->HasWriteSymbol(value))
					{
						sprintf(buffer, "%s", pSymbolStore->GetWriteSymbol(value).c_str());
						pLine->flags |= kFlagSymbol;
					}
					else
					{	
						sprintf(buffer, "$%02x%02x", pLine->param2, pLine->param1);
					}
				}
				else
				{
					sprintf(buffer, "$%02x%02x", pLine->param2, pLine->param1);		
				}
#endif
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeAbsoluteX:
				// TODO: need some symbol output here
				sprintf(buffer, "$%02x%02x,X", pLine->param2, pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeAbsoluteY:
				// TODO: need some symbol output here
				sprintf(buffer, "$%02x%02x,Y", pLine->param2, pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeAccumulator:
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeImmediate:
				sprintf(buffer, "#$%02x", pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeImplied:
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeIndirect:
				sprintf(buffer, "($%02x%02x)", pLine->param2, pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeIndirectX:
				sprintf(buffer, "($%02x,X)", pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeIndirectY:
				sprintf(buffer, "($%02x),Y", pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeRelative:
				addr = pLine->address + 2 + (int8_t)pLine->param1;	// offset is from next instruction, hence the '+2'
				// is this a new label ?
				if(!pSymbolStore->HasLabelSymbol(addr))
				{
					sprintf(buffer, "<label%04x>", addr);
					pSymbolStore->AddAutoLabel(addr, buffer);
				}

//				if(pLine->memoryOp == Cpu6502::EMemoryOp::kNone)
//				{
//					if(pSymbolStore->HasLabelSymbol(addr))
//					{
//						sprintf(buffer, "%s", pSymbolStore->GetLabelSymbol(addr).c_str());
//						pLine->flags |= kFlagLabel;
//					}
//					else
//					{	
//						sprintf(buffer, "$%04x", addr);
//					}		
//				}
//				else
//				{
					sprintf(buffer, "$%04x", addr);
//				}
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeZeroPage:
//				if(pLine->memoryOp == Cpu6502::EMemoryOp::kRead)
//				{
//					if(pSymbolStore->HasReadSymbol(pLine->param1))
//					{
//						sprintf(buffer, "%s", pSymbolStore->GetReadSymbol(pLine->param1).c_str());
//						pLine->flags |= kFlagSymbol;
//					}
//					else
//					{	
//						sprintf(buffer, "$%02x", pLine->param1);
//					}
//				}
//				else if(pLine->memoryOp == Cpu6502::EMemoryOp::kWrite)
//				{
//					if(pSymbolStore->HasWriteSymbol(pLine->param1))
//					{
//						sprintf(buffer, "%s", pSymbolStore->GetWriteSymbol(pLine->param1).c_str());
//						pLine->flags |= kFlagSymbol;
//					}
//					else
//					{	
//						sprintf(buffer, "$%02x", pLine->param1);
//					}
//				}
//				else
//				{
					sprintf(buffer, "$%02x", pLine->param1);
//				}
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeZeroPageX:
				sprintf(buffer, "$%02x,X", pLine->param1);
				break;
			case Cpu6502Base::EAddressingMode::kAddrModeZeroPageY:
				sprintf(buffer, "$%02x,Y", pLine->param1);
				break;
			default:
				break;
		}
		pLine->detail = buffer;
	}
}

void Disassembler::AddObviousLabels()
{
	uint16_t addr;
	for(uint32_t iLine=0 ; iLine<lines.size() ; iLine++)
	{
		char buffer[128];
			
		// Based on mnemonic
		switch(lines[iLine].mnemonic)
		{
			
			case Cpu6502Base::kMnemonic_RTS:
				sprintf(buffer, "<label%04x>", lines[iLine].address+1);
				pSymbolStore->AddAutoLabel(lines[iLine].address+1, buffer);
				break;
			case Cpu6502Base::kMnemonic_JMP:
				// add label after a jump
				sprintf(buffer, "<label%04x>", lines[iLine].address+3);
				pSymbolStore->AddAutoLabel(lines[iLine].address+3, buffer);
				
				// add a label to the jump location
				addr = lines[iLine].param2 << 8 | lines[iLine].param1;
				sprintf(buffer, "<label%04x>", addr);
				pSymbolStore->AddAutoLabel(addr, buffer);
				break;
			default:
				break;
		}
		
		// based on addressing addressingMode
		switch(lines[iLine].addressingMode)
		{
			case Cpu6502Base::EAddressingMode::kAddrModeAbsolute:
			case Cpu6502Base::EAddressingMode::kAddrModeAbsoluteX:
			case Cpu6502Base::EAddressingMode::kAddrModeAbsoluteY:
				addr = lines[iLine].param2 << 8 | lines[iLine].param1;
				sprintf(buffer, "<label%04x>", addr);
				pSymbolStore->AddAutoLabel(addr, buffer);
				break;
			default:
				break;
		}
	}

	UpdateDetailLines();
	UpdateLineLabels();
}

void Disassembler::UpdateLineLabels()
{
	for(uint32_t iLine=0 ; iLine<lines.size() ; iLine++)
	{
		if(pSymbolStore->HasLabelSymbol(lines[iLine].address))
		{
			lines[iLine].label = pSymbolStore->GetLabelSymbol(lines[iLine].address);
		}
	}
}

bool Disassembler::HandleCommand(const std::shared_ptr<CommandBase> command)
{
#if 0
	UpdateLineLabels();
	UpdateDetailLines();
#endif
	return true;
}

int Disassembler::GetNumLines()
{
	return lines.size();
}

const Disassembler::Line & Disassembler::GetLine(int num)
{
	return lines[num];
}

