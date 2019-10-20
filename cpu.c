#include <stdio.h>
#include "cpu.h"

struct cpu_mnemonic {
	bool	valid;
	uint8_t	value;
	uint8_t	mnemonic;
	uint8_t	addrMode;
	bool	undocumented;
};

static struct cpu_mnemonic mnemonics[256];
static char* mnemonicStrings[kCpuNumMnemonics];

static void cpu_AddMnemonic(uint8_t value, uint8_t mnemonic, uint8_t addrMode, _Bool documented)
{
	mnemonics[value].valid = true;
	mnemonics[value].mnemonic = mnemonic;
}

static void cpu_AddEmptyMnemonic(uint8_t value)
{
	mnemonics[value].valid = false;
}

void cpu_Init(void)
{
	printf("Intialising 6502 CPU\n");
	// init mnemonics

	mnemonicStrings[kCpuMnemonic_ADC] = "ADC";
	mnemonicStrings[kCpuMnemonic_TYA] = "TYA";

	for(int i=0 ; i<256 ; i++)
	{
		cpu_AddEmptyMnemonic((uint8_t)i);
	}

	// ADC
	cpu_AddMnemonic(0x69, kCpuMnemonic_ADC, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0x65, kCpuMnemonic_ADC, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0x75, kCpuMnemonic_ADC, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0x6d, kCpuMnemonic_ADC, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0x7d, kCpuMnemonic_ADC, kCpuAddrModeAbsoluteX, true);
	cpu_AddMnemonic(0x79, kCpuMnemonic_ADC, kCpuAddrModeAbsoluteY, true);
	cpu_AddMnemonic(0x61, kCpuMnemonic_ADC, kCpuAddrModeIndirectX, true);
	cpu_AddMnemonic(0x71, kCpuMnemonic_ADC, kCpuAddrModeIndirectY, true);

	printf("Intialised 6502 CPU\n");
}

void cpu_DumpInfo(void)
{
	// output the CPU info we have...
	printf("=====================================================================================\n");
	printf("    |0x00|0x01|0x02|0x03|0x04|0x05|0x06|0x07|0x08|0x09|0x0a|0x0b|0x0c|0x0d|0x0e|0x0f|\n");
	printf("=====================================================================================\n");
	for(int row=0 ; row < 16 ; row++)
	{
		printf("0x%01x0|", row);
		for(int col=0 ; col<16 ; col++)
		{
			uint8_t operand = row*16 + col;
			if(mnemonics[operand].valid)
			{
				printf("%s |", mnemonicStrings[mnemonics[operand].mnemonic]);
			}
			else
			{
				printf("    |");
			}
		}
		printf("\n");
		printf("=====================================================================================\n");
	}
}

void cpu_dumpDisassembly(uint16_t address, uint16_t size)
{
}
