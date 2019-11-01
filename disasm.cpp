#include <string>
#include <iomanip>
#include <sstream>

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
	// get rid of the old disassembly
	
	lines.clear();
	
	uint32_t currentAddress = address;
	while(currentAddress < (uint32_t)address + (uint32_t)size)
	{
		Line thisLine;
		std::stringstream ss;
		ss << std::hex << currentAddress;
		thisLine.address = ss.str();
		
		uint8_t opcode = pMemory->Read(currentAddress);
		const Cpu6502::Opcode* opcodeInfo = pCpu->GetOpcode(opcode);
		
		switch(opcodeInfo->addrMode)
		{
			default:
				break;
		}
		
		currentAddress+=opcodeInfo->length;
	}
	return kError_OK;
}

void Disassembler::DumpToTTY(void)
{
}
