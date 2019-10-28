#include <stdio.h>
#include <string.h>
#include "disasm.h"
#include "log.h"

#define kMaxLabelSize 20
#define kMaxMnemonicSize 20
#define kMaxAddressSize 6
#define kMaxBytesSize 8
#define kMaxDisasmLines 4096

struct disasm_line
{
	char label[kMaxLabelSize+1];		// max size plus null
	char address[kMaxAddressSize+1];	// '0x0000' plus null
	char bytes[kMaxBytesSize+1];		// '00 00 00' plus null
	char mnemonic[kMaxMnemonicSize+1];	// max size plus null
};

static struct disasm_line disasmLines[kMaxDisasmLines];

void disasm_Init(void)
{
	LOG("disasm_Init()\n");

	for(int i=0 ; i<kMaxDisasmLines ; i++)
	{
		// clear the lines
		memset(&disasmLines[i].label, 0, kMaxLabelSize+1);
		memset(&disasmLines[i].address, 0, kMaxAddressSize+1);
		memset(&disasmLines[i].bytes, 0, kMaxBytesSize+1);
		memset(&disasmLines[i].mnemonic, 0, kMaxMnemonicSize+1);
	}
}

errorcode_t disasm_Disassemble(uint16_t address, uint16_t size, uint16_t org)
{
	return ERROR_OK;
}

void disasm_DumpToTTY(void)
{
}

