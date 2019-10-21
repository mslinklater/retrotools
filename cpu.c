#include <stdio.h>
#include "cpu.h"

enum eCpuAddressingMode {
	kCpuAddrModeAccumulator,
	kCpuAddrModeImmediate,
	kCpuAddrModeZeroPage,
	kCpuAddrModeZeroPageX,
	kCpuAddrModeZeroPageY,
	kCpuAddrModeAbsolute,
	kCpuAddrModeAbsoluteX,
	kCpuAddrModeAbsoluteY,
	kCpuAddrModeIndirectX,
	kCpuAddrModeIndirectY,
	kCpuAddrModeRelative,
	kCpuAddrModeImplied,
	kCpuAddrModeIndirect
};

enum eCpuMnemonic {
	kCpuMnemonic_ADC = 0,
	kCpuMnemonic_AND,
	kCpuMnemonic_ASL,
	kCpuMnemonic_BCC,
	kCpuMnemonic_BCS,
	kCpuMnemonic_BEQ,
	kCpuMnemonic_BIT,
	kCpuMnemonic_BMI,
	kCpuMnemonic_BNE,
	kCpuMnemonic_BPL,
	kCpuMnemonic_BRK,
	kCpuMnemonic_BVC,
	kCpuMnemonic_BVS,
	kCpuMnemonic_CLC,
	kCpuMnemonic_CLD,
	kCpuMnemonic_CLI,
	kCpuMnemonic_CLV,
	kCpuMnemonic_CMP,
	kCpuMnemonic_CPX,
	kCpuMnemonic_CPY,
	kCpuMnemonic_DEC,
	kCpuMnemonic_DEX,
	kCpuMnemonic_DEY,
	kCpuMnemonic_EOR,
	kCpuMnemonic_INC,
	kCpuMnemonic_INX,
	kCpuMnemonic_INY,
	kCpuMnemonic_JMP,
	kCpuMnemonic_JSR,
	kCpuMnemonic_LDA,
	kCpuMnemonic_LDX,
	kCpuMnemonic_LDY,
	kCpuMnemonic_LSR,
	kCpuMnemonic_NOP,
	kCpuMnemonic_ORA,
	kCpuMnemonic_PHA,
	kCpuMnemonic_PHP,
	kCpuMnemonic_PLA,
	kCpuMnemonic_ROL,
	kCpuMnemonic_ROR,
	kCpuMnemonic_RTI,
	kCpuMnemonic_RTS,
	kCpuMnemonic_SBC,
	kCpuMnemonic_SEC,
	kCpuMnemonic_SED,
	kCpuMnemonic_SEI,
	kCpuMnemonic_STA,
	kCpuMnemonic_STX,
	kCpuMnemonic_STY,
	kCpuMnemonic_TAX,
	kCpuMnemonic_TAY,
	kCpuMnemonic_TSX,
	kCpuMnemonic_TXA,
	kCpuMnemonic_TXS,
	kCpuMnemonic_TYA,
	kCpuMnemonic_Num
};

struct cpu_mnemonic {
	bool					valid;
	uint8_t					value;
	enum eCpuMnemonic		mnemonic;
	enum eCpuAddressingMode	addrMode;
	bool					undocumented;
};

static struct cpu_mnemonic mnemonics[256];
static char* mnemonicStrings[kCpuMnemonic_Num];

static void cpu_AddMnemonic(uint8_t value, enum eCpuMnemonic mnemonic, enum eCpuAddressingMode addrMode, _Bool documented)
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
	mnemonicStrings[kCpuMnemonic_AND] = "AND";
	mnemonicStrings[kCpuMnemonic_ASL] = "ASL";
	mnemonicStrings[kCpuMnemonic_BCC] = "BCC";
	mnemonicStrings[kCpuMnemonic_BCS] = "BCS";
	mnemonicStrings[kCpuMnemonic_BEQ] = "BEQ";
	mnemonicStrings[kCpuMnemonic_BIT] = "BIT";
	mnemonicStrings[kCpuMnemonic_BMI] = "BMI";
	mnemonicStrings[kCpuMnemonic_BNE] = "BNE";
	mnemonicStrings[kCpuMnemonic_BPL] = "BPL";
	mnemonicStrings[kCpuMnemonic_BRK] = "BRK";
	mnemonicStrings[kCpuMnemonic_BVC] = "BVC";
	mnemonicStrings[kCpuMnemonic_BVS] = "BVS";
	mnemonicStrings[kCpuMnemonic_CLC] = "CLC";
	mnemonicStrings[kCpuMnemonic_CLD] = "CLD";
	mnemonicStrings[kCpuMnemonic_CLI] = "CLI";
	mnemonicStrings[kCpuMnemonic_CLV] = "CLV";
	mnemonicStrings[kCpuMnemonic_CMP] = "CMP";
	mnemonicStrings[kCpuMnemonic_CPX] = "CPX";
	mnemonicStrings[kCpuMnemonic_CPY] = "CPY";
	mnemonicStrings[kCpuMnemonic_DEC] = "DEC";
	mnemonicStrings[kCpuMnemonic_DEX] = "DEX";
	mnemonicStrings[kCpuMnemonic_DEY] = "DEY";
	mnemonicStrings[kCpuMnemonic_EOR] = "EOR";
	mnemonicStrings[kCpuMnemonic_INC] = "INC";
	mnemonicStrings[kCpuMnemonic_INX] = "INX";
	mnemonicStrings[kCpuMnemonic_INY] = "INY";
	mnemonicStrings[kCpuMnemonic_JMP] = "JMP";
	mnemonicStrings[kCpuMnemonic_JSR] = "JSR";
	mnemonicStrings[kCpuMnemonic_LDA] = "LDA";
	mnemonicStrings[kCpuMnemonic_LDX] = "LDX";
	mnemonicStrings[kCpuMnemonic_LDY] = "LDY";
	mnemonicStrings[kCpuMnemonic_LSR] = "LSR";
	mnemonicStrings[kCpuMnemonic_NOP] = "NOP";
	mnemonicStrings[kCpuMnemonic_ORA] = "ORA";
	mnemonicStrings[kCpuMnemonic_PHA] = "PHA";
	mnemonicStrings[kCpuMnemonic_PHP] = "PHP";
	mnemonicStrings[kCpuMnemonic_PLA] = "PLA";
	mnemonicStrings[kCpuMnemonic_ROL] = "ROL";
	mnemonicStrings[kCpuMnemonic_ROR] = "ROR";
	mnemonicStrings[kCpuMnemonic_RTI] = "RTI";
	mnemonicStrings[kCpuMnemonic_RTS] = "RTS";
	mnemonicStrings[kCpuMnemonic_SBC] = "SBC";
	mnemonicStrings[kCpuMnemonic_SEC] = "SEC";
	mnemonicStrings[kCpuMnemonic_SED] = "SED";
	mnemonicStrings[kCpuMnemonic_SEI] = "SEI";
	mnemonicStrings[kCpuMnemonic_STA] = "STA";
	mnemonicStrings[kCpuMnemonic_STX] = "STX";
	mnemonicStrings[kCpuMnemonic_STY] = "STY";
	mnemonicStrings[kCpuMnemonic_TAX] = "TAX";
	mnemonicStrings[kCpuMnemonic_TAY] = "TAY";
	mnemonicStrings[kCpuMnemonic_TSX] = "TSX";
	mnemonicStrings[kCpuMnemonic_TXA] = "TXA";
	mnemonicStrings[kCpuMnemonic_TXS] = "TXS";
	mnemonicStrings[kCpuMnemonic_TYA] = "TYA";

	// Lets fill it up with empty mnemonics first
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

	// AND
	cpu_AddMnemonic(0x29, kCpuMnemonic_AND, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0x25, kCpuMnemonic_AND, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0x35, kCpuMnemonic_AND, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0x2d, kCpuMnemonic_AND, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0x3d, kCpuMnemonic_AND, kCpuAddrModeAbsoluteX, true);
	cpu_AddMnemonic(0x39, kCpuMnemonic_AND, kCpuAddrModeAbsoluteY, true);
	cpu_AddMnemonic(0x21, kCpuMnemonic_AND, kCpuAddrModeIndirectX, true);
	cpu_AddMnemonic(0x31, kCpuMnemonic_AND, kCpuAddrModeIndirectY, true);

	// ASL
	cpu_AddMnemonic(0x0a, kCpuMnemonic_ASL, kCpuAddrModeAccumulator, true);
	cpu_AddMnemonic(0x06, kCpuMnemonic_ASL, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0x16, kCpuMnemonic_ASL, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0x0e, kCpuMnemonic_ASL, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0x1e, kCpuMnemonic_ASL, kCpuAddrModeAbsoluteX, true);

	//BCC
	cpu_AddMnemonic(0x90, kCpuMnemonic_BCC, kCpuAddrModeRelative, true);

	// BCS
	cpu_AddMnemonic(0xb0, kCpuMnemonic_BCS, kCpuAddrModeRelative, true);

	// BEQ
	cpu_AddMnemonic(0xf0, kCpuMnemonic_BEQ, kCpuAddrModeRelative, true);
	
	// BIT
	cpu_AddMnemonic(0x24, kCpuMnemonic_BIT, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0x2c, kCpuMnemonic_BIT, kCpuAddrModeAbsolute, true);

	// BMI
	cpu_AddMnemonic(0x30, kCpuMnemonic_BMI, kCpuAddrModeRelative, true);

	// BNE
	cpu_AddMnemonic(0xd0, kCpuMnemonic_BNE, kCpuAddrModeRelative, true);

	// BPL
	cpu_AddMnemonic(0x10, kCpuMnemonic_BPL, kCpuAddrModeRelative, true);

	// BRK
	cpu_AddMnemonic(0x00, kCpuMnemonic_BRK, kCpuAddrModeImplied, true);

	// BVC
	cpu_AddMnemonic(0x50, kCpuMnemonic_BVC, kCpuAddrModeRelative, true);

	//BVS
	cpu_AddMnemonic(0x70, kCpuMnemonic_BVS, kCpuAddrModeRelative, true);

	// CLC
	cpu_AddMnemonic(0x18, kCpuMnemonic_CLC, kCpuAddrModeRelative, true);

	// CLD
	cpu_AddMnemonic(0xd8, kCpuMnemonic_CLD, kCpuAddrModeRelative, true);

	// CLI
	cpu_AddMnemonic(0x58, kCpuMnemonic_CLI, kCpuAddrModeRelative, true);

	// CLV
	cpu_AddMnemonic(0xb8, kCpuMnemonic_CLV, kCpuAddrModeRelative, true);

	// CMP
	cpu_AddMnemonic(0xc9, kCpuMnemonic_CMP, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0xc5, kCpuMnemonic_CMP, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xd5, kCpuMnemonic_CMP, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0xcd, kCpuMnemonic_CMP, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0xdd, kCpuMnemonic_CMP, kCpuAddrModeAbsoluteX, true);
	cpu_AddMnemonic(0xd9, kCpuMnemonic_CMP, kCpuAddrModeAbsoluteY, true);
	cpu_AddMnemonic(0xc1, kCpuMnemonic_CMP, kCpuAddrModeIndirectX, true);
	cpu_AddMnemonic(0xd1, kCpuMnemonic_CMP, kCpuAddrModeIndirectY, true);

	// CPX
	cpu_AddMnemonic(0xe0, kCpuMnemonic_CPX, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0xe4, kCpuMnemonic_CPX, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xec, kCpuMnemonic_CPX, kCpuAddrModeAbsolute, true);

	// CPY
	cpu_AddMnemonic(0xc0, kCpuMnemonic_CPY, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0xc4, kCpuMnemonic_CPY, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xcc, kCpuMnemonic_CPY, kCpuAddrModeAbsolute, true);

	// DEC
	cpu_AddMnemonic(0xc6, kCpuMnemonic_DEC, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xd6, kCpuMnemonic_DEC, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0xce, kCpuMnemonic_DEC, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0xde, kCpuMnemonic_DEC, kCpuAddrModeAbsoluteX, true);

	// DEX
	cpu_AddMnemonic(0xca, kCpuMnemonic_DEX, kCpuAddrModeImplied, true);

	// DEY
	cpu_AddMnemonic(0x88, kCpuMnemonic_DEY, kCpuAddrModeImplied, true);

	// EOR
	cpu_AddMnemonic(0x49, kCpuMnemonic_EOR, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0x45, kCpuMnemonic_EOR, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0x55, kCpuMnemonic_EOR, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0x4d, kCpuMnemonic_EOR, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0x5d, kCpuMnemonic_EOR, kCpuAddrModeAbsoluteX, true);
	cpu_AddMnemonic(0x59, kCpuMnemonic_EOR, kCpuAddrModeAbsoluteY, true);
	cpu_AddMnemonic(0x41, kCpuMnemonic_EOR, kCpuAddrModeIndirectX, true);
	cpu_AddMnemonic(0x51, kCpuMnemonic_EOR, kCpuAddrModeIndirectY, true);

	// INC
	cpu_AddMnemonic(0xe6, kCpuMnemonic_INC, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xf6, kCpuMnemonic_INC, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0xee, kCpuMnemonic_INC, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0xfe, kCpuMnemonic_INC, kCpuAddrModeAbsoluteX, true);

	// INX
	cpu_AddMnemonic(0xe8, kCpuMnemonic_INX, kCpuAddrModeImplied, true);

	// INY
	cpu_AddMnemonic(0xc8, kCpuMnemonic_INY, kCpuAddrModeImplied, true);

	// JMP
	cpu_AddMnemonic(0x4c, kCpuMnemonic_JMP, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0x2c, kCpuMnemonic_JMP, kCpuAddrModeIndirect, true);

	// JSR
	cpu_AddMnemonic(0x20, kCpuMnemonic_JSR, kCpuAddrModeAbsolute, true);

	// LDA
	cpu_AddMnemonic(0xa9, kCpuMnemonic_LDA, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0xa5, kCpuMnemonic_LDA, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xb5, kCpuMnemonic_LDA, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0xad, kCpuMnemonic_LDA, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0xbd, kCpuMnemonic_LDA, kCpuAddrModeAbsoluteX, true);
	cpu_AddMnemonic(0xb9, kCpuMnemonic_LDA, kCpuAddrModeAbsoluteY, true);
	cpu_AddMnemonic(0xa1, kCpuMnemonic_LDA, kCpuAddrModeIndirectX, true);
	cpu_AddMnemonic(0xb1, kCpuMnemonic_LDA, kCpuAddrModeIndirectY, true);

	// LDX
	cpu_AddMnemonic(0xa2, kCpuMnemonic_LDX, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0xa6, kCpuMnemonic_LDX, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xb6, kCpuMnemonic_LDX, kCpuAddrModeZeroPageY, true);
	cpu_AddMnemonic(0xae, kCpuMnemonic_LDX, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0xbe, kCpuMnemonic_LDX, kCpuAddrModeAbsoluteY, true);

	// LDY
	cpu_AddMnemonic(0xa0, kCpuMnemonic_LDY, kCpuAddrModeImmediate, true);
	cpu_AddMnemonic(0xa4, kCpuMnemonic_LDY, kCpuAddrModeZeroPage, true);
	cpu_AddMnemonic(0xb4, kCpuMnemonic_LDY, kCpuAddrModeZeroPageX, true);
	cpu_AddMnemonic(0xac, kCpuMnemonic_LDY, kCpuAddrModeAbsolute, true);
	cpu_AddMnemonic(0xbc, kCpuMnemonic_LDY, kCpuAddrModeAbsoluteX, true);

	// LSR
	cpu_AddMnemonic(0xf0, kCpuMnemonic_BEQ, kCpuAddrModeRelative, true);
	cpu_AddMnemonic(0xf0, kCpuMnemonic_BEQ, kCpuAddrModeRelative, true);
	cpu_AddMnemonic(0xf0, kCpuMnemonic_BEQ, kCpuAddrModeRelative, true);
	cpu_AddMnemonic(0xf0, kCpuMnemonic_BEQ, kCpuAddrModeRelative, true);







	printf("Intialised 6502 CPU\n");
}

void cpu_DumpInfo(void)
{
	// output the CPU info we have...
	printf("-------------------------------------------------------------------------------------\n");
	printf("    |0x00|0x01|0x02|0x03|0x04|0x05|0x06|0x07|0x08|0x09|0x0a|0x0b|0x0c|0x0d|0x0e|0x0f|\n");
	printf("-------------------------------------------------------------------------------------\n");
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
		printf("-------------------------------------------------------------------------------------\n");
	}
}

void cpu_dumpDisassembly(uint16_t address, uint16_t size)
{
}
