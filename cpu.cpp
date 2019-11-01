//#include <stdio.h>
#include "cpu.h"
#include "memory.h"
#include "vcs.h"


#define CPU_STATUS_MASK_CARRY		0x01
#define CPU_STATUS_MASK_ZERO		0x02
#define CPU_STATUS_MASK_INTERRPUT	0x04
#define CPU_STATUS_MASK_DECIMAL		0x08
#define CPU_STATUS_MASK_BREAK		0x10
#define CPU_STATUS_MASK_OVERFLOW	0x40
#define CPU_STATUS_MASK_NEGATIVE	0x80

// Constructor

Cpu6502::Cpu6502()
{
}

uint8_t cpu_disasm_implied(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	printf("0x%04x %02x       %s\n", address, opcode, mnemonic);
	return 1;
}

uint8_t cpu_disasm_accumulator(uint16_t address, const char* mnemonic)
{
//	printf("0x%04x %02x       %s\n", address, memory_Read(address), mnemonic);
	return 1;
}

uint8_t cpu_disasm_immediate(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t val = memory_Read(address+1);
//	printf("0x%04x %02x %02x    %s #$%02x\n", address, opcode, val, mnemonic, val);
	return 2;
}

uint8_t cpu_disasm_zeropage(uint16_t address, const char* mnemonic)
{
//	uint8_t location = memory_Read(address+1);
//	const struct vcs_info* pInfo = vcs_getInfo(location);
//	uint8_t opcode = memory_Read(address);
//	uint8_t val = memory_Read(address+1);

//	if(pInfo->writeName.length() != 0)
//	{
//		// read or write here
//		printf("0x%04x %02x %02x    %s %s\n", address, opcode, val, mnemonic, pInfo->writeName.c_str());
//	}
//	else
//	{
//		printf("0x%04x %02x %02x    %s $%02x\n", address, opcode, val, mnemonic, memory_Read(address+1));
//	}
	return 2;
}

uint8_t cpu_disasm_zeropagex(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t val = memory_Read(address+1);
//	printf("0x%04x %02x %02x    %s $%02x,X\n", address, opcode, val, mnemonic, memory_Read(address+1));
	return 2;
}

uint8_t cpu_disasm_zeropagey(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t val = memory_Read(address+1);
//	printf("0x%04x %02x %02x    %s $%02x,Y\n", address, opcode, val, mnemonic, memory_Read(address+1));
	return 2;
}

uint8_t cpu_disasm_absolute(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	uint8_t last = memory_Read(address+2);
//	printf("0x%04x %02x %02x %02x %s $%02x%02x\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}

uint8_t cpu_disasm_absolutex(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	uint8_t last = memory_Read(address+2);
//	printf("0x%04x %02x %02x %02x %s $%02x%02x,X\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}
 
uint8_t cpu_disasm_absolutey(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	uint8_t last = memory_Read(address+2);
//	printf("0x%04x %02x %02x %02x %s $%02x%02x,Y\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}
 
uint8_t cpu_disasm_indirectx(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	printf("0x%04x %02x %02x    %s ($%02x,X)\n", address, opcode, next, mnemonic, memory_Read(address+1));
	return 2;
}
 
uint8_t cpu_disasm_indirecty(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	printf("0x%04x %02x %02x    %s ($%02x),Y\n", address, opcode, next, mnemonic, memory_Read(address+1));
	return 2;
}
 
uint8_t cpu_disasm_relative(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	printf("0x%04x %02x %02x    %s $%04x(#$%02x)\n", address, opcode, next, mnemonic, address+memory_Read(address+1), memory_Read(address+1));
	return 2;
}

uint8_t cpu_disasm_indirect(uint16_t address, const char* mnemonic)
{
//	uint8_t opcode = memory_Read(address);
//	uint8_t next = memory_Read(address+1);
//	uint8_t last = memory_Read(address+2);
//	printf("0x%04x %02x %02x %02x %s ($%02x%02x)\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}

void Cpu6502::AddOpcode(uint8_t value, enum eMnemonic mnemonic, enum eAddressingMode addrMode, bool documented)
{
	opcodes[value].valid = true;
	opcodes[value].mnemonic = mnemonic;
	opcodes[value].addrMode = addrMode;

	switch(addrMode)
	{
		case kAddrModeAccumulator: 
			opcodes[value].length = 1;
			break;
		case kAddrModeImmediate: 
			opcodes[value].length = 2;
			break;
		case kAddrModeZeroPage: 
			opcodes[value].length = 2;
			break;
		case kAddrModeZeroPageX: 
			opcodes[value].length = 2;
			break;
		case kAddrModeZeroPageY: 
			opcodes[value].length = 2;
			break;
		case kAddrModeAbsolute: 
			opcodes[value].length = 3;
			break;
		case kAddrModeAbsoluteX: 
			opcodes[value].length = 3;
			break;
		case kAddrModeAbsoluteY: 
			opcodes[value].length = 3;
			break;
		case kAddrModeIndirectX: 
			opcodes[value].length = 2;
			break;
		case kAddrModeIndirectY: 
			opcodes[value].length = 2;
			break;
		case kAddrModeRelative: 
			opcodes[value].length = 2;
			break;
		case kAddrModeImplied: 
			opcodes[value].length = 1;
			break;
		case kAddrModeIndirect: 
			opcodes[value].length = 2;
			break;
		default:
			break;
	}
}

void Cpu6502::AddEmptyOpcode(uint8_t value)
{
	opcodes[value].valid = false;
}

const Cpu6502::Opcode* Cpu6502::GetOpcode(uint16_t opcode) const
{
	return &opcodes[opcode];
}

void Cpu6502::Init(eVariant variant)
{
	printf("Intialising 6502 CPU\n");
	// init mnemonics

	mnemonicStrings[kMnemonic_ADC] = "ADC";
	mnemonicStrings[kMnemonic_AND] = "AND";
	mnemonicStrings[kMnemonic_ASL] = "ASL";
	mnemonicStrings[kMnemonic_BCC] = "BCC";
	mnemonicStrings[kMnemonic_BEQ] = "BEQ";
	mnemonicStrings[kMnemonic_BCS] = "BCS";
	mnemonicStrings[kMnemonic_BIT] = "BIT";
	mnemonicStrings[kMnemonic_BMI] = "BMI";
	mnemonicStrings[kMnemonic_BNE] = "BNE";
	mnemonicStrings[kMnemonic_BPL] = "BPL";
	mnemonicStrings[kMnemonic_BRK] = "BRK";
	mnemonicStrings[kMnemonic_BVC] = "BVC";
	mnemonicStrings[kMnemonic_BVS] = "BVS";
	mnemonicStrings[kMnemonic_CLC] = "CLC";
	mnemonicStrings[kMnemonic_CLD] = "CLD";
	mnemonicStrings[kMnemonic_CLI] = "CLI";
	mnemonicStrings[kMnemonic_CLV] = "CLV";
	mnemonicStrings[kMnemonic_CMP] = "CMP";
	mnemonicStrings[kMnemonic_CPX] = "CPX";
	mnemonicStrings[kMnemonic_CPY] = "CPY";
	mnemonicStrings[kMnemonic_DEC] = "DEC";
	mnemonicStrings[kMnemonic_DEX] = "DEX";
	mnemonicStrings[kMnemonic_DEY] = "DEY";
	mnemonicStrings[kMnemonic_EOR] = "EOR";
	mnemonicStrings[kMnemonic_INC] = "INC";
	mnemonicStrings[kMnemonic_INX] = "INX";
	mnemonicStrings[kMnemonic_INY] = "INY";
	mnemonicStrings[kMnemonic_JMP] = "JMP";
	mnemonicStrings[kMnemonic_JSR] = "JSR";
	mnemonicStrings[kMnemonic_LDA] = "LDA";
	mnemonicStrings[kMnemonic_LDX] = "LDX";
	mnemonicStrings[kMnemonic_LDY] = "LDY";
	mnemonicStrings[kMnemonic_LSR] = "LSR";
	mnemonicStrings[kMnemonic_NOP] = "NOP";
	mnemonicStrings[kMnemonic_ORA] = "ORA";
	mnemonicStrings[kMnemonic_PHA] = "PHA";
	mnemonicStrings[kMnemonic_PHP] = "PHP";
	mnemonicStrings[kMnemonic_PLA] = "PLA";
	mnemonicStrings[kMnemonic_ROL] = "ROL";
	mnemonicStrings[kMnemonic_ROR] = "ROR";
	mnemonicStrings[kMnemonic_RTI] = "RTI";
	mnemonicStrings[kMnemonic_RTS] = "RTS";
	mnemonicStrings[kMnemonic_SBC] = "SBC";
	mnemonicStrings[kMnemonic_SEC] = "SEC";
	mnemonicStrings[kMnemonic_SED] = "SED";
	mnemonicStrings[kMnemonic_SEI] = "SEI";
	mnemonicStrings[kMnemonic_STA] = "STA";
	mnemonicStrings[kMnemonic_STX] = "STX";
	mnemonicStrings[kMnemonic_STY] = "STY";
	mnemonicStrings[kMnemonic_TAX] = "TAX";
	mnemonicStrings[kMnemonic_TAY] = "TAY";
	mnemonicStrings[kMnemonic_TSX] = "TSX";
	mnemonicStrings[kMnemonic_TXA] = "TXA";
	mnemonicStrings[kMnemonic_TXS] = "TXS";
	mnemonicStrings[kMnemonic_TYA] = "TYA";

	addrModeStrings[kAddrModeAccumulator] = " Acc ";
	addrModeStrings[kAddrModeImmediate] = " Imm ";
	addrModeStrings[kAddrModeZeroPage] = "ZeroP";
	addrModeStrings[kAddrModeZeroPageX] = "ZeroX";
	addrModeStrings[kAddrModeZeroPageY] = "ZeroY";
	addrModeStrings[kAddrModeAbsolute] = " Abs ";
	addrModeStrings[kAddrModeAbsoluteX] = " AbsX";
	addrModeStrings[kAddrModeAbsoluteY] = " AbsY";
	addrModeStrings[kAddrModeIndirectX] = " IndX";
	addrModeStrings[kAddrModeIndirectY] = " IndY";
	addrModeStrings[kAddrModeRelative] = " Rel ";
	addrModeStrings[kAddrModeImplied] = " Imp ";
	addrModeStrings[kAddrModeIndirect] = " Ind ";
    
	// Lets fill it up with empty mnemonics first
	for(int i=0 ; i<256 ; i++)
	{
		AddEmptyOpcode((uint8_t)i);
	}

	// ADC
	AddOpcode(0x69, kMnemonic_ADC, kAddrModeImmediate, true);
	AddOpcode(0x65, kMnemonic_ADC, kAddrModeZeroPage, true);
	AddOpcode(0x75, kMnemonic_ADC, kAddrModeZeroPageX, true);
	AddOpcode(0x6d, kMnemonic_ADC, kAddrModeAbsolute, true);
	AddOpcode(0x7d, kMnemonic_ADC, kAddrModeAbsoluteX, true);
	AddOpcode(0x79, kMnemonic_ADC, kAddrModeAbsoluteY, true);
	AddOpcode(0x61, kMnemonic_ADC, kAddrModeIndirectX, true);
	AddOpcode(0x71, kMnemonic_ADC, kAddrModeIndirectY, true);

	// AND
	AddOpcode(0x29, kMnemonic_AND, kAddrModeImmediate, true);
	AddOpcode(0x25, kMnemonic_AND, kAddrModeZeroPage, true);
	AddOpcode(0x35, kMnemonic_AND, kAddrModeZeroPageX, true);
	AddOpcode(0x2d, kMnemonic_AND, kAddrModeAbsolute, true);
	AddOpcode(0x3d, kMnemonic_AND, kAddrModeAbsoluteX, true);
	AddOpcode(0x39, kMnemonic_AND, kAddrModeAbsoluteY, true);
	AddOpcode(0x21, kMnemonic_AND, kAddrModeIndirectX, true);
	AddOpcode(0x31, kMnemonic_AND, kAddrModeIndirectY, true);

	// ASL
	AddOpcode(0x0a, kMnemonic_ASL, kAddrModeAccumulator, true);
	AddOpcode(0x06, kMnemonic_ASL, kAddrModeZeroPage, true);
	AddOpcode(0x16, kMnemonic_ASL, kAddrModeZeroPageX, true);
	AddOpcode(0x0e, kMnemonic_ASL, kAddrModeAbsolute, true);
	AddOpcode(0x1e, kMnemonic_ASL, kAddrModeAbsoluteX, true);

	//BCC
	AddOpcode(0x90, kMnemonic_BCC, kAddrModeRelative, true);

	// BCS
	AddOpcode(0xb0, kMnemonic_BCS, kAddrModeRelative, true);

	// BEQ
	AddOpcode(0xf0, kMnemonic_BEQ, kAddrModeRelative, true);
	
	// BIT
	AddOpcode(0x24, kMnemonic_BIT, kAddrModeZeroPage, true);
	AddOpcode(0x2c, kMnemonic_BIT, kAddrModeAbsolute, true);

	// BMI
	AddOpcode(0x30, kMnemonic_BMI, kAddrModeRelative, true);

	// BNE
	AddOpcode(0xd0, kMnemonic_BNE, kAddrModeRelative, true);

	// BPL
	AddOpcode(0x10, kMnemonic_BPL, kAddrModeRelative, true);

	// BRK
	AddOpcode(0x00, kMnemonic_BRK, kAddrModeImplied, true);

	// BVC
	AddOpcode(0x50, kMnemonic_BVC, kAddrModeRelative, true);

	//BVS
	AddOpcode(0x70, kMnemonic_BVS, kAddrModeRelative, true);

	// CLC
	AddOpcode(0x18, kMnemonic_CLC, kAddrModeRelative, true);

	// CLD
	AddOpcode(0xd8, kMnemonic_CLD, kAddrModeImplied, true);

	// CLI
	AddOpcode(0x58, kMnemonic_CLI, kAddrModeRelative, true);

	// CLV
	AddOpcode(0xb8, kMnemonic_CLV, kAddrModeRelative, true);

	// CMP
	AddOpcode(0xc9, kMnemonic_CMP, kAddrModeImmediate, true);
	AddOpcode(0xc5, kMnemonic_CMP, kAddrModeZeroPage, true);
	AddOpcode(0xd5, kMnemonic_CMP, kAddrModeZeroPageX, true);
	AddOpcode(0xcd, kMnemonic_CMP, kAddrModeAbsolute, true);
	AddOpcode(0xdd, kMnemonic_CMP, kAddrModeAbsoluteX, true);
	AddOpcode(0xd9, kMnemonic_CMP, kAddrModeAbsoluteY, true);
	AddOpcode(0xc1, kMnemonic_CMP, kAddrModeIndirectX, true);
	AddOpcode(0xd1, kMnemonic_CMP, kAddrModeIndirectY, true);

	// CPX
	AddOpcode(0xe0, kMnemonic_CPX, kAddrModeImmediate, true);
	AddOpcode(0xe4, kMnemonic_CPX, kAddrModeZeroPage, true);
	AddOpcode(0xec, kMnemonic_CPX, kAddrModeAbsolute, true);
	// CPY
	AddOpcode(0xc0, kMnemonic_CPY, kAddrModeImmediate, true);
	AddOpcode(0xc4, kMnemonic_CPY, kAddrModeZeroPage, true);
	AddOpcode(0xcc, kMnemonic_CPY, kAddrModeAbsolute, true);

	// DEC
	AddOpcode(0xc6, kMnemonic_DEC, kAddrModeZeroPage, true);
	AddOpcode(0xd6, kMnemonic_DEC, kAddrModeZeroPageX, true);
	AddOpcode(0xce, kMnemonic_DEC, kAddrModeAbsolute, true);
	AddOpcode(0xde, kMnemonic_DEC, kAddrModeAbsoluteX, true);

	// DEX
	AddOpcode(0xca, kMnemonic_DEX, kAddrModeImplied, true);

	// DEY
	AddOpcode(0x88, kMnemonic_DEY, kAddrModeImplied, true);

	// EOR
	AddOpcode(0x49, kMnemonic_EOR, kAddrModeImmediate, true);
	AddOpcode(0x45, kMnemonic_EOR, kAddrModeZeroPage, true);
	AddOpcode(0x55, kMnemonic_EOR, kAddrModeZeroPageX, true);
	AddOpcode(0x4d, kMnemonic_EOR, kAddrModeAbsolute, true);
	AddOpcode(0x5d, kMnemonic_EOR, kAddrModeAbsoluteX, true);
	AddOpcode(0x59, kMnemonic_EOR, kAddrModeAbsoluteY, true);
	AddOpcode(0x41, kMnemonic_EOR, kAddrModeIndirectX, true);
	AddOpcode(0x51, kMnemonic_EOR, kAddrModeIndirectY, true);

	// INC
	AddOpcode(0xe6, kMnemonic_INC, kAddrModeZeroPage, true);
	AddOpcode(0xf6, kMnemonic_INC, kAddrModeZeroPageX, true);
	AddOpcode(0xee, kMnemonic_INC, kAddrModeAbsolute, true);
	AddOpcode(0xfe, kMnemonic_INC, kAddrModeAbsoluteX, true);

	// INX
	AddOpcode(0xe8, kMnemonic_INX, kAddrModeImplied, true);

	// INY
	AddOpcode(0xc8, kMnemonic_INY, kAddrModeImplied, true);

	// JMP
	AddOpcode(0x4c, kMnemonic_JMP, kAddrModeAbsolute, true);
	AddOpcode(0x2c, kMnemonic_JMP, kAddrModeIndirect, true);

	// JSR
	AddOpcode(0x20, kMnemonic_JSR, kAddrModeAbsolute, true);

	// LDA
	AddOpcode(0xa9, kMnemonic_LDA, kAddrModeImmediate, true);
	AddOpcode(0xa5, kMnemonic_LDA, kAddrModeZeroPage, true);
	AddOpcode(0xb5, kMnemonic_LDA, kAddrModeZeroPageX, true);
	AddOpcode(0xad, kMnemonic_LDA, kAddrModeAbsolute, true);
	AddOpcode(0xbd, kMnemonic_LDA, kAddrModeAbsoluteX, true);
	AddOpcode(0xb9, kMnemonic_LDA, kAddrModeAbsoluteY, true);
	AddOpcode(0xa1, kMnemonic_LDA, kAddrModeIndirectX, true);
	AddOpcode(0xb1, kMnemonic_LDA, kAddrModeIndirectY, true);

	// LDX
	AddOpcode(0xa2, kMnemonic_LDX, kAddrModeImmediate, true);
	AddOpcode(0xa6, kMnemonic_LDX, kAddrModeZeroPage, true);
	AddOpcode(0xb6, kMnemonic_LDX, kAddrModeZeroPageY, true);
	AddOpcode(0xae, kMnemonic_LDX, kAddrModeAbsolute, true);
	AddOpcode(0xbe, kMnemonic_LDX, kAddrModeAbsoluteY, true);

	// LDY
	AddOpcode(0xa0, kMnemonic_LDY, kAddrModeImmediate, true);
	AddOpcode(0xa4, kMnemonic_LDY, kAddrModeZeroPage, true);
	AddOpcode(0xb4, kMnemonic_LDY, kAddrModeZeroPageX, true);
	AddOpcode(0xac, kMnemonic_LDY, kAddrModeAbsolute, true);
	AddOpcode(0xbc, kMnemonic_LDY, kAddrModeAbsoluteX, true);

	// LSR
	AddOpcode(0x4a, kMnemonic_LSR, kAddrModeAccumulator, true);
	AddOpcode(0x46, kMnemonic_LSR, kAddrModeZeroPage, true);
	AddOpcode(0x56, kMnemonic_LSR, kAddrModeZeroPageX, true);
	AddOpcode(0x4e, kMnemonic_LSR, kAddrModeAbsolute, true);
	AddOpcode(0x5e, kMnemonic_LSR, kAddrModeAbsoluteX, true);

	// NOP
	AddOpcode(0xea, kMnemonic_NOP, kAddrModeImplied, true);

	// ORA
	AddOpcode(0x09, kMnemonic_ORA, kAddrModeImmediate, true);
	AddOpcode(0x05, kMnemonic_ORA, kAddrModeZeroPage, true);
	AddOpcode(0x15, kMnemonic_ORA, kAddrModeZeroPageX, true);
	AddOpcode(0x0d, kMnemonic_ORA, kAddrModeAbsolute, true);
	AddOpcode(0x1d, kMnemonic_ORA, kAddrModeAbsoluteX, true);
	AddOpcode(0x19, kMnemonic_ORA, kAddrModeAbsoluteY, true);
	AddOpcode(0x01, kMnemonic_ORA, kAddrModeIndirectX, true);
	AddOpcode(0x11, kMnemonic_ORA, kAddrModeIndirectY, true);

	// PHA
	AddOpcode(0x48, kMnemonic_PHA, kAddrModeImplied, true);

	// PHP
	AddOpcode(0x08, kMnemonic_PHP, kAddrModeImplied, true);

	// PLA
	AddOpcode(0x68, kMnemonic_PLA, kAddrModeImplied, true);

	// ROL
	AddOpcode(0x2a, kMnemonic_ROL, kAddrModeAccumulator, true);
	AddOpcode(0x26, kMnemonic_ROL, kAddrModeZeroPage, true);
	AddOpcode(0x36, kMnemonic_ROL, kAddrModeZeroPageX, true);
	AddOpcode(0x2e, kMnemonic_ROL, kAddrModeAbsolute, true);
	AddOpcode(0x3e, kMnemonic_ROL, kAddrModeAbsoluteX, true);

	// ROR
	AddOpcode(0x6a, kMnemonic_ROR, kAddrModeAccumulator, true);
	AddOpcode(0x66, kMnemonic_ROR, kAddrModeZeroPage, true);
	AddOpcode(0x76, kMnemonic_ROR, kAddrModeZeroPageX, true);
	AddOpcode(0x6e, kMnemonic_ROR, kAddrModeAbsolute, true);
	AddOpcode(0x7e, kMnemonic_ROR, kAddrModeAbsoluteX, true);

	// RTI
	AddOpcode(0x40, kMnemonic_RTI, kAddrModeImplied, true);

	// RTS
	AddOpcode(0x60, kMnemonic_RTS, kAddrModeImplied, true);

	// SBC
	AddOpcode(0xe9, kMnemonic_SBC, kAddrModeImmediate, true);
	AddOpcode(0xe5, kMnemonic_SBC, kAddrModeZeroPage, true);
	AddOpcode(0xf5, kMnemonic_SBC, kAddrModeZeroPageX, true);
	AddOpcode(0xed, kMnemonic_SBC, kAddrModeAbsolute, true);
	AddOpcode(0xfd, kMnemonic_SBC, kAddrModeAbsoluteX, true);
	AddOpcode(0xf9, kMnemonic_SBC, kAddrModeAbsoluteY, true);
	AddOpcode(0xe1, kMnemonic_SBC, kAddrModeIndirectX, true);
	AddOpcode(0xf1, kMnemonic_SBC, kAddrModeIndirectY, true);

	// SEC
	AddOpcode(0x38, kMnemonic_SEC, kAddrModeImplied, true);

	// SED
	AddOpcode(0xf8, kMnemonic_SED, kAddrModeImplied, true);

	// SEI
	AddOpcode(0x78, kMnemonic_SEI, kAddrModeImplied, true);

	// STA
	AddOpcode(0x85, kMnemonic_STA, kAddrModeZeroPage, true);
	AddOpcode(0x95, kMnemonic_STA, kAddrModeZeroPageX, true);
	AddOpcode(0x8d, kMnemonic_STA, kAddrModeAbsolute, true);
	AddOpcode(0x9d, kMnemonic_STA, kAddrModeAbsoluteX, true);
	AddOpcode(0x99, kMnemonic_STA, kAddrModeAbsoluteY, true);
	AddOpcode(0x81, kMnemonic_STA, kAddrModeIndirectX, true);
	AddOpcode(0x91, kMnemonic_STA, kAddrModeIndirectY, true);

	// STX
	AddOpcode(0x86, kMnemonic_STX, kAddrModeZeroPage, true);
	AddOpcode(0x96, kMnemonic_STX, kAddrModeZeroPageY, true);
	AddOpcode(0x8e, kMnemonic_STX, kAddrModeAbsolute, true);

	// STY
	AddOpcode(0x84, kMnemonic_STY, kAddrModeZeroPage, true);
	AddOpcode(0x94, kMnemonic_STY, kAddrModeZeroPageX, true);
	AddOpcode(0x8c, kMnemonic_STY, kAddrModeAbsolute, true);

	// TAX
	AddOpcode(0xaa, kMnemonic_TAX, kAddrModeImplied, true);

	// TAY
	AddOpcode(0xa8, kMnemonic_TAX, kAddrModeImplied, true);

	// TSX
	AddOpcode(0xba, kMnemonic_TSX, kAddrModeImplied, true);

	// TXA
	AddOpcode(0x8a, kMnemonic_TXA, kAddrModeImplied, true);

	// TXS
	AddOpcode(0x9a, kMnemonic_TXS, kAddrModeImplied, true);

	// TYA
	AddOpcode(0x98, kMnemonic_TYA, kAddrModeImplied, true);

	printf("Intialised 6502 CPU\n");
}

void Cpu6502::DumpInfo(void)
{
	// output the CPU info we have...
	printf("-----------------------------------------------------------------------------------------------------\n");
	printf("    | 0x00| 0x01| 0x02| 0x03| 0x04| 0x05| 0x06| 0x07| 0x08| 0x09| 0x0a| 0x0b| 0x0c| 0x0d| 0x0e| 0x0f|\n");
	printf("-----------------------------------------------------------------------------------------------------\n");
	for(int row=0 ; row < 16 ; row++)
	{
		printf("0x%01x0|", row);
		for(int col=0 ; col<16 ; col++)
		{
			uint8_t operand = row*16 + col;
			if(opcodes[operand].valid)
			{
				printf(" %s |", mnemonicStrings[opcodes[operand].mnemonic].c_str());
			}
			else
			{
				printf("     |");
			}
		}
		printf("\n");
		printf("0x%01x0|", row);
		for(int col=0 ; col<16 ; col++)
		{
			uint8_t operand = row*16 + col;
			if(opcodes[operand].valid)
			{
				printf("%s|", addrModeStrings[opcodes[operand].addrMode].c_str());
			}
			else
			{
				printf("     |");
			}
		}
		printf("\n");
		printf("-----------------------------------------------------------------------------------------------------\n");
	}
}
