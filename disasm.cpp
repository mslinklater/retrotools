//#include <stdio.h>
//#include <stdbool.h>

#include <string>

#include "disasm.h"
#include "log.h"

//#define kMaxLabelSize 20
//#define kMaxMnemonicSize 20
//#define kMaxAddressSize 6
//#define kMaxBytesSize 8
#define kMaxDisasmLines 4096

struct disasm_line
{
	bool used;
	std::string label;		// max size plus null
	std::string address;	// '0x0000' plus null
	std::string bytes;		// '00 00 00' plus null
	std::string mnemonic;	// max size plus null
};

static struct disasm_line disasmLines[kMaxDisasmLines];

void disasm_Init(void)
{
	LOG("disasm_Init()\n");

	for(int i=0 ; i<kMaxDisasmLines ; i++)
	{
		disasmLines[i].used = false;
	}
}

eErrorCode disasm_Disassemble(uint16_t address, uint16_t size, uint16_t org)
{
	return kError_OK;
}

void disasm_DumpToTTY(void)
{
}

