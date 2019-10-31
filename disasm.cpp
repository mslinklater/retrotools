#include <string>

#include "disasm.h"
#include "log.h"

static const uint32_t kMaxDisasmLines = 4096;

Disassembler::Disassembler()
{
}

Disassembler::~Disassembler()
{
}

struct disasm_line
{
	bool used;
	std::string label;		// max size plus null
	std::string address;	// '0x0000' plus null
	std::string bytes;		// '00 00 00' plus null
	std::string mnemonic;	// max size plus null
};

static struct disasm_line disasmLines[kMaxDisasmLines];

void Disassembler::Init(void)
{
	LOG("disasm_Init()\n");

	for(int i=0 ; i<kMaxDisasmLines ; i++)
	{
		disasmLines[i].used = false;
	}
}

eErrorCode Disassembler::Disassemble(uint16_t address, uint16_t size, uint16_t org)
{
	return kError_OK;
}

void Disassembler::DumpToTTY(void)
{
}
