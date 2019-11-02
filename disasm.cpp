#include <string>
#include <sstream>
#include <cassert>
#include "disasm.h"
#include "log.h"
#include "cpu.h"
#include "memory.h"

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
	LOG("Disassembler::Init()\n");

}

void Disassembler::SetMemory(Memory* mem)
{
	pMemory = mem;
}

void Disassembler::SetCpu(Cpu6502* cpu)
{
	pCpu = cpu;
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
		
		// address
		sprintf(buffer, "%04x", currentAddress);
		thisLine.address = buffer;
		
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
			thisLine.mnemonic = buffer;
			
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
					sprintf(buffer, "$%02x", plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeZeroPageX:
					sprintf(buffer, "$%02x,X", plus1);
					break;
				case Cpu6502::eAddressingMode::kAddrModeZeroPageY:
					sprintf(buffer, "$%02x,Y", plus1);
					break;
			}
			thisLine.detail = buffer;
			currentAddress+=opcodeInfo->length;
		}
		else
		{
			currentAddress++;	  
		}
		lines.push_back(thisLine);
	}
	return kError_OK;
}

void Disassembler::DumpToTTY(void)
{
	// dump disassembly lines to DumpToTTY
	for(Line line : lines)
	{
		printf("%s %s %s %s\n", line.address.c_str(), line.bytes.c_str(), line.mnemonic.c_str(), line.detail.c_str());
	}
}
