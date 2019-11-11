#include <string>
#include <sstream>
#include <cassert>
#include "disasm.h"
#include "log.h"
#include "symbolstore.h"
#include "components/cpu6502.h"
#include "components/memory.h"

static const uint32_t kMaxDisasmLines = 4096;

Disassembler::Disassembler()
{
}

Disassembler::~Disassembler()
{
}

//static struct disasm_line disasmLines[kMaxDisasmLines];

void Disassembler::Init(void)
{
	LOGINFO("Disassembler::Init()\n");

}

void Disassembler::SetMemory(Memory* mem)
{
	pMemory = mem;
}

void Disassembler::SetCpu(Cpu6502* cpu)
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
	anonLabelCount = 0;
	
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
		
		uint8_t opcode = pMemory->Read(currentAddress);
		const Cpu6502::Opcode* opcodeInfo = pCpu->GetOpcode(opcode);

		if(opcodeInfo->valid)
		{
			// bytes
			uint8_t plus1 = pMemory->Read(currentAddress+1);
			uint8_t plus2 = pMemory->Read(currentAddress+2);
			
			switch(opcodeInfo->length)
			{
				case 3:
					sprintf(buffer, "%02x %02x %02x", opcode, plus1, plus2);
					break;
				case 2:
					sprintf(buffer, "%02x %02x   ", opcode, plus1);
					break;
				default:
					sprintf(buffer, "%02x      ", opcode);
					break;
			}
			thisLine.bytes = buffer;

			// opcode
			Cpu6502::eMnemonic mnemonic = pCpu->GetOpcode(opcode)->mnemonic;
			sprintf(buffer, "%s", pCpu->GetMnemonicString(mnemonic).c_str());
			thisLine.mnemonicString = buffer;
			thisLine.mnemonic = mnemonic;
			
			// detail
			sprintf(buffer, " ");
			uint16_t addr;
			switch(opcodeInfo->addrMode)
			{
				case Cpu6502::eAddressingMode::kAddrModeAbsolute:
					sprintf(buffer, "$%02x%02x", plus2, plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeAbsoluteX:
					sprintf(buffer, "$%02x%02x,X", plus2, plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeAbsoluteY:
					sprintf(buffer, "$%02x%02x,Y", plus2, plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeAccumulator:
					break;
				case Cpu6502::eAddressingMode::kAddrModeImmediate:
					sprintf(buffer, "#$%02x", plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeImplied:
					break;
				case Cpu6502::eAddressingMode::kAddrModeIndirect:
					sprintf(buffer, "($%02x%02x)", plus2, plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeIndirectX:
					sprintf(buffer, "($%02x,X)", plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeIndirectY:
					sprintf(buffer, "($%02x),Y", plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeRelative:
					addr = currentAddress + (int8_t)plus1;
					sprintf(buffer, "$%04x", addr);
					break;
				case Cpu6502::eAddressingMode::kAddrModeZeroPage:
					if(opcodeInfo->memoryOp == Cpu6502::eMemoryOp::kRead)
					{
						if(pSymbolStore->HasReadSymbol(plus1))
						{
							sprintf(buffer, "%s", pSymbolStore->GetReadSymbol(plus1).c_str());
							thisLine.flags |= kFlagSymbol;
						}
						else
						{	
							sprintf(buffer, "$%02x", plus1);		  
						}
					}
					else if(opcodeInfo->memoryOp == Cpu6502::eMemoryOp::kWrite)
					{
						if(pSymbolStore->HasWriteSymbol(plus1))
						{
							sprintf(buffer, "%s", pSymbolStore->GetWriteSymbol(plus1).c_str());
							thisLine.flags |= kFlagSymbol;
						}
						else
						{	
							sprintf(buffer, "$%02x", plus1);		  
						}
					}
					else
					{
						sprintf(buffer, "$%02x", plus1);						
					}
					break;
				case Cpu6502::eAddressingMode::kAddrModeZeroPageX:
					sprintf(buffer, "$%02x,X", plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeZeroPageY:
					sprintf(buffer, "$%02x,Y", plus1);
					break;
				default:
					break;
			}
			thisLine.detail = buffer;
			
			// Move to the next instruction
			currentAddress+=opcodeInfo->length;
		}
		else
		{
			currentAddress++;	  
		}
		lines.push_back(thisLine);
	}

	AddPostRTSLabels();
	
	return kError_OK;
}

void Disassembler::AddPostRTSLabels()
{
	for(uint32_t iLine=0 ; iLine<lines.size() ; iLine++)
	{
		switch(lines[iLine].mnemonic)
		{
			char buffer[128];
			
			case Cpu6502::kMnemonic_RTS:
				sprintf(buffer, "<label%d>", anonLabelCount++);
				lines[iLine+1].label = buffer;
				pSymbolStore->AddLabel(lines[iLine].address, buffer);
				break;
			default:
				break;
		}
	}
}


int Disassembler::GetNumLines()
{
	return lines.size();
}

const Disassembler::Line & Disassembler::GetLine(int num)
{
	return lines[num];
}

