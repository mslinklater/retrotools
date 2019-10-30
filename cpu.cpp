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

// statics

bool				Cpu6502::staticsInitialised;
Cpu6502::Mnemonic	Cpu6502::mnemonics[256];
std::string			Cpu6502::mnemonicStrings[kCpuMnemonic_Num];
std::string			Cpu6502::addrModeStrings[kCpuAddrMode_Num];

// Constructor

Cpu6502::Cpu6502()
{
}

uint8_t cpu_disasm_implied(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	printf("0x%04x %02x       %s\n", address, opcode, mnemonic);
	return 1;
}

uint8_t cpu_disasm_accumulator(uint16_t address, const char* mnemonic)
{
	printf("0x%04x %02x       %s\n", address, memory_Read(address), mnemonic);
	return 1;
}

uint8_t cpu_disasm_immediate(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t val = memory_Read(address+1);
	printf("0x%04x %02x %02x    %s #$%02x\n", address, opcode, val, mnemonic, val);
	return 2;
}

uint8_t cpu_disasm_zeropage(uint16_t address, const char* mnemonic)
{
	uint8_t location = memory_Read(address+1);
	const struct vcs_info* pInfo = vcs_getInfo(location);
	uint8_t opcode = memory_Read(address);
	uint8_t val = memory_Read(address+1);

	if(pInfo->writeName.length() != 0)
	{
		// read or write here
		printf("0x%04x %02x %02x    %s %s\n", address, opcode, val, mnemonic, pInfo->writeName.c_str());
	}
	else
	{
		printf("0x%04x %02x %02x    %s $%02x\n", address, opcode, val, mnemonic, memory_Read(address+1));
	}
	return 2;
}

uint8_t cpu_disasm_zeropagex(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t val = memory_Read(address+1);
	printf("0x%04x %02x %02x    %s $%02x,X\n", address, opcode, val, mnemonic, memory_Read(address+1));
	return 2;
}

uint8_t cpu_disasm_zeropagey(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t val = memory_Read(address+1);
	printf("0x%04x %02x %02x    %s $%02x,Y\n", address, opcode, val, mnemonic, memory_Read(address+1));
	return 2;
}

uint8_t cpu_disasm_absolute(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	uint8_t last = memory_Read(address+2);
	printf("0x%04x %02x %02x %02x %s $%02x%02x\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}

uint8_t cpu_disasm_absolutex(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	uint8_t last = memory_Read(address+2);
	printf("0x%04x %02x %02x %02x %s $%02x%02x,X\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}
 
uint8_t cpu_disasm_absolutey(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	uint8_t last = memory_Read(address+2);
	printf("0x%04x %02x %02x %02x %s $%02x%02x,Y\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}
 
uint8_t cpu_disasm_indirectx(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	printf("0x%04x %02x %02x    %s ($%02x,X)\n", address, opcode, next, mnemonic, memory_Read(address+1));
	return 2;
}
 
uint8_t cpu_disasm_indirecty(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	printf("0x%04x %02x %02x    %s ($%02x),Y\n", address, opcode, next, mnemonic, memory_Read(address+1));
	return 2;
}
 
uint8_t cpu_disasm_relative(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	printf("0x%04x %02x %02x    %s $%04x(#$%02x)\n", address, opcode, next, mnemonic, address+memory_Read(address+1), memory_Read(address+1));
	return 2;
}

uint8_t cpu_disasm_indirect(uint16_t address, const char* mnemonic)
{
	uint8_t opcode = memory_Read(address);
	uint8_t next = memory_Read(address+1);
	uint8_t last = memory_Read(address+2);
	printf("0x%04x %02x %02x %02x %s ($%02x%02x)\n", address, opcode, next, last, mnemonic, memory_Read(address+2), memory_Read(address+1));
	return 3;
}

void Cpu6502::AddMnemonic(uint8_t value, enum eMnemonic mnemonic, enum eAddressingMode addrMode, bool documented)
{
	mnemonics[value].valid = true;
	mnemonics[value].mnemonic = mnemonic;
	mnemonics[value].addrMode = addrMode;

	switch(addrMode)
	{
		case kCpuAddrModeAccumulator: 
			mnemonics[value].pfDisasm = cpu_disasm_accumulator; 
			break;
		case kCpuAddrModeImmediate: 
			mnemonics[value].pfDisasm = cpu_disasm_immediate; 
			break;
		case kCpuAddrModeZeroPage: 
			mnemonics[value].pfDisasm = cpu_disasm_zeropage; 
			break;
		case kCpuAddrModeZeroPageX: 
			mnemonics[value].pfDisasm = cpu_disasm_zeropagex; 
			break;
		case kCpuAddrModeZeroPageY: 
			mnemonics[value].pfDisasm = cpu_disasm_zeropagey; 
			break;
		case kCpuAddrModeAbsolute: 
			mnemonics[value].pfDisasm = cpu_disasm_absolute; 
			break;
		case kCpuAddrModeAbsoluteX: 
			mnemonics[value].pfDisasm = cpu_disasm_absolutex; 
			break;
		case kCpuAddrModeAbsoluteY: 
			mnemonics[value].pfDisasm = cpu_disasm_absolutey; 
			break;
		case kCpuAddrModeIndirectX: 
			mnemonics[value].pfDisasm = cpu_disasm_indirectx; 
			break;
		case kCpuAddrModeIndirectY: 
			mnemonics[value].pfDisasm = cpu_disasm_indirecty; 
			break;
		case kCpuAddrModeRelative: 
			mnemonics[value].pfDisasm = cpu_disasm_relative; 
			break;
		case kCpuAddrModeImplied: 
			mnemonics[value].pfDisasm = cpu_disasm_implied; 
			break;
		case kCpuAddrModeIndirect: 
			mnemonics[value].pfDisasm = cpu_disasm_indirect; 
			break;
		default:
			break;
	}
}

void Cpu6502::AddEmptyMnemonic(uint8_t value)
{
	mnemonics[value].valid = false;
	mnemonics[value].pfDisasm = 0;
}


void Cpu6502::Init(eVariant variant)
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

	addrModeStrings[kCpuAddrModeAccumulator] = " Acc ";
	addrModeStrings[kCpuAddrModeImmediate] = " Imm ";
	addrModeStrings[kCpuAddrModeZeroPage] = "ZeroP";
	addrModeStrings[kCpuAddrModeZeroPageX] = "ZeroX";
	addrModeStrings[kCpuAddrModeZeroPageY] = "ZeroY";
	addrModeStrings[kCpuAddrModeAbsolute] = " Abs ";
	addrModeStrings[kCpuAddrModeAbsoluteX] = " AbsX";
	addrModeStrings[kCpuAddrModeAbsoluteY] = " AbsY";
	addrModeStrings[kCpuAddrModeIndirectX] = " IndX";
	addrModeStrings[kCpuAddrModeIndirectY] = " IndY";
	addrModeStrings[kCpuAddrModeRelative] = " Rel ";
	addrModeStrings[kCpuAddrModeImplied] = " Imp ";
	addrModeStrings[kCpuAddrModeIndirect] = " Ind ";
    
	// Lets fill it up with empty mnemonics first
	for(int i=0 ; i<256 ; i++)
	{
		AddEmptyMnemonic((uint8_t)i);
	}

	// ADC
	AddMnemonic(0x69, kCpuMnemonic_ADC, kCpuAddrModeImmediate, true);
	AddMnemonic(0x65, kCpuMnemonic_ADC, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x75, kCpuMnemonic_ADC, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x6d, kCpuMnemonic_ADC, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x7d, kCpuMnemonic_ADC, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0x79, kCpuMnemonic_ADC, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0x61, kCpuMnemonic_ADC, kCpuAddrModeIndirectX, true);
	AddMnemonic(0x71, kCpuMnemonic_ADC, kCpuAddrModeIndirectY, true);

	// AND
	AddMnemonic(0x29, kCpuMnemonic_AND, kCpuAddrModeImmediate, true);
	AddMnemonic(0x25, kCpuMnemonic_AND, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x35, kCpuMnemonic_AND, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x2d, kCpuMnemonic_AND, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x3d, kCpuMnemonic_AND, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0x39, kCpuMnemonic_AND, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0x21, kCpuMnemonic_AND, kCpuAddrModeIndirectX, true);
	AddMnemonic(0x31, kCpuMnemonic_AND, kCpuAddrModeIndirectY, true);

	// ASL
	AddMnemonic(0x0a, kCpuMnemonic_ASL, kCpuAddrModeAccumulator, true);
	AddMnemonic(0x06, kCpuMnemonic_ASL, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x16, kCpuMnemonic_ASL, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x0e, kCpuMnemonic_ASL, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x1e, kCpuMnemonic_ASL, kCpuAddrModeAbsoluteX, true);

	//BCC
	AddMnemonic(0x90, kCpuMnemonic_BCC, kCpuAddrModeRelative, true);

	// BCS
	AddMnemonic(0xb0, kCpuMnemonic_BCS, kCpuAddrModeRelative, true);

	// BEQ
	AddMnemonic(0xf0, kCpuMnemonic_BEQ, kCpuAddrModeRelative, true);
	
	// BIT
	AddMnemonic(0x24, kCpuMnemonic_BIT, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x2c, kCpuMnemonic_BIT, kCpuAddrModeAbsolute, true);

	// BMI
	AddMnemonic(0x30, kCpuMnemonic_BMI, kCpuAddrModeRelative, true);

	// BNE
	AddMnemonic(0xd0, kCpuMnemonic_BNE, kCpuAddrModeRelative, true);

	// BPL
	AddMnemonic(0x10, kCpuMnemonic_BPL, kCpuAddrModeRelative, true);

	// BRK
	AddMnemonic(0x00, kCpuMnemonic_BRK, kCpuAddrModeImplied, true);

	// BVC
	AddMnemonic(0x50, kCpuMnemonic_BVC, kCpuAddrModeRelative, true);

	//BVS
	AddMnemonic(0x70, kCpuMnemonic_BVS, kCpuAddrModeRelative, true);

	// CLC
	AddMnemonic(0x18, kCpuMnemonic_CLC, kCpuAddrModeRelative, true);

	// CLD
	AddMnemonic(0xd8, kCpuMnemonic_CLD, kCpuAddrModeImplied, true);

	// CLI
	AddMnemonic(0x58, kCpuMnemonic_CLI, kCpuAddrModeRelative, true);

	// CLV
	AddMnemonic(0xb8, kCpuMnemonic_CLV, kCpuAddrModeRelative, true);

	// CMP
	AddMnemonic(0xc9, kCpuMnemonic_CMP, kCpuAddrModeImmediate, true);
	AddMnemonic(0xc5, kCpuMnemonic_CMP, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xd5, kCpuMnemonic_CMP, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0xcd, kCpuMnemonic_CMP, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xdd, kCpuMnemonic_CMP, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0xd9, kCpuMnemonic_CMP, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0xc1, kCpuMnemonic_CMP, kCpuAddrModeIndirectX, true);
	AddMnemonic(0xd1, kCpuMnemonic_CMP, kCpuAddrModeIndirectY, true);

	// CPX
	AddMnemonic(0xe0, kCpuMnemonic_CPX, kCpuAddrModeImmediate, true);
	AddMnemonic(0xe4, kCpuMnemonic_CPX, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xec, kCpuMnemonic_CPX, kCpuAddrModeAbsolute, true);

	// CPY
	AddMnemonic(0xc0, kCpuMnemonic_CPY, kCpuAddrModeImmediate, true);
	AddMnemonic(0xc4, kCpuMnemonic_CPY, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xcc, kCpuMnemonic_CPY, kCpuAddrModeAbsolute, true);

	// DEC
	AddMnemonic(0xc6, kCpuMnemonic_DEC, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xd6, kCpuMnemonic_DEC, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0xce, kCpuMnemonic_DEC, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xde, kCpuMnemonic_DEC, kCpuAddrModeAbsoluteX, true);

	// DEX
	AddMnemonic(0xca, kCpuMnemonic_DEX, kCpuAddrModeImplied, true);

	// DEY
	AddMnemonic(0x88, kCpuMnemonic_DEY, kCpuAddrModeImplied, true);

	// EOR
	AddMnemonic(0x49, kCpuMnemonic_EOR, kCpuAddrModeImmediate, true);
	AddMnemonic(0x45, kCpuMnemonic_EOR, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x55, kCpuMnemonic_EOR, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x4d, kCpuMnemonic_EOR, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x5d, kCpuMnemonic_EOR, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0x59, kCpuMnemonic_EOR, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0x41, kCpuMnemonic_EOR, kCpuAddrModeIndirectX, true);
	AddMnemonic(0x51, kCpuMnemonic_EOR, kCpuAddrModeIndirectY, true);

	// INC
	AddMnemonic(0xe6, kCpuMnemonic_INC, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xf6, kCpuMnemonic_INC, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0xee, kCpuMnemonic_INC, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xfe, kCpuMnemonic_INC, kCpuAddrModeAbsoluteX, true);

	// INX
	AddMnemonic(0xe8, kCpuMnemonic_INX, kCpuAddrModeImplied, true);

	// INY
	AddMnemonic(0xc8, kCpuMnemonic_INY, kCpuAddrModeImplied, true);

	// JMP
	AddMnemonic(0x4c, kCpuMnemonic_JMP, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x2c, kCpuMnemonic_JMP, kCpuAddrModeIndirect, true);

	// JSR
	AddMnemonic(0x20, kCpuMnemonic_JSR, kCpuAddrModeAbsolute, true);

	// LDA
	AddMnemonic(0xa9, kCpuMnemonic_LDA, kCpuAddrModeImmediate, true);
	AddMnemonic(0xa5, kCpuMnemonic_LDA, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xb5, kCpuMnemonic_LDA, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0xad, kCpuMnemonic_LDA, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xbd, kCpuMnemonic_LDA, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0xb9, kCpuMnemonic_LDA, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0xa1, kCpuMnemonic_LDA, kCpuAddrModeIndirectX, true);
	AddMnemonic(0xb1, kCpuMnemonic_LDA, kCpuAddrModeIndirectY, true);

	// LDX
	AddMnemonic(0xa2, kCpuMnemonic_LDX, kCpuAddrModeImmediate, true);
	AddMnemonic(0xa6, kCpuMnemonic_LDX, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xb6, kCpuMnemonic_LDX, kCpuAddrModeZeroPageY, true);
	AddMnemonic(0xae, kCpuMnemonic_LDX, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xbe, kCpuMnemonic_LDX, kCpuAddrModeAbsoluteY, true);

	// LDY
	AddMnemonic(0xa0, kCpuMnemonic_LDY, kCpuAddrModeImmediate, true);
	AddMnemonic(0xa4, kCpuMnemonic_LDY, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xb4, kCpuMnemonic_LDY, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0xac, kCpuMnemonic_LDY, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xbc, kCpuMnemonic_LDY, kCpuAddrModeAbsoluteX, true);

	// LSR
	AddMnemonic(0x4a, kCpuMnemonic_LSR, kCpuAddrModeAccumulator, true);
	AddMnemonic(0x46, kCpuMnemonic_LSR, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x56, kCpuMnemonic_LSR, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x4e, kCpuMnemonic_LSR, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x5e, kCpuMnemonic_LSR, kCpuAddrModeAbsoluteX, true);

	// NOP
	AddMnemonic(0xea, kCpuMnemonic_NOP, kCpuAddrModeImplied, true);

	// ORA
	AddMnemonic(0x09, kCpuMnemonic_ORA, kCpuAddrModeImmediate, true);
	AddMnemonic(0x05, kCpuMnemonic_ORA, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x15, kCpuMnemonic_ORA, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x0d, kCpuMnemonic_ORA, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x1d, kCpuMnemonic_ORA, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0x19, kCpuMnemonic_ORA, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0x01, kCpuMnemonic_ORA, kCpuAddrModeIndirectX, true);
	AddMnemonic(0x11, kCpuMnemonic_ORA, kCpuAddrModeIndirectY, true);

	// PHA
	AddMnemonic(0x48, kCpuMnemonic_PHA, kCpuAddrModeImplied, true);

	// PHP
	AddMnemonic(0x08, kCpuMnemonic_PHP, kCpuAddrModeImplied, true);

	// PLA
	AddMnemonic(0x68, kCpuMnemonic_PLA, kCpuAddrModeImplied, true);

	// ROL
	AddMnemonic(0x2a, kCpuMnemonic_ROL, kCpuAddrModeAccumulator, true);
	AddMnemonic(0x26, kCpuMnemonic_ROL, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x36, kCpuMnemonic_ROL, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x2e, kCpuMnemonic_ROL, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x3e, kCpuMnemonic_ROL, kCpuAddrModeAbsoluteX, true);

	// ROR
	AddMnemonic(0x6a, kCpuMnemonic_ROR, kCpuAddrModeAccumulator, true);
	AddMnemonic(0x66, kCpuMnemonic_ROR, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x76, kCpuMnemonic_ROR, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x6e, kCpuMnemonic_ROR, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x7e, kCpuMnemonic_ROR, kCpuAddrModeAbsoluteX, true);

	// RTI
	AddMnemonic(0x40, kCpuMnemonic_RTI, kCpuAddrModeImplied, true);

	// RTS
	AddMnemonic(0x60, kCpuMnemonic_RTS, kCpuAddrModeImplied, true);

	// SBC
	AddMnemonic(0xe9, kCpuMnemonic_SBC, kCpuAddrModeImmediate, true);
	AddMnemonic(0xe5, kCpuMnemonic_SBC, kCpuAddrModeZeroPage, true);
	AddMnemonic(0xf5, kCpuMnemonic_SBC, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0xed, kCpuMnemonic_SBC, kCpuAddrModeAbsolute, true);
	AddMnemonic(0xfd, kCpuMnemonic_SBC, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0xf9, kCpuMnemonic_SBC, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0xe1, kCpuMnemonic_SBC, kCpuAddrModeIndirectX, true);
	AddMnemonic(0xf1, kCpuMnemonic_SBC, kCpuAddrModeIndirectY, true);

	// SEC
	AddMnemonic(0x38, kCpuMnemonic_SEC, kCpuAddrModeImplied, true);

	// SED
	AddMnemonic(0xf8, kCpuMnemonic_SED, kCpuAddrModeImplied, true);

	// SEI
	AddMnemonic(0x78, kCpuMnemonic_SEI, kCpuAddrModeImplied, true);

	// STA
	AddMnemonic(0x85, kCpuMnemonic_STA, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x95, kCpuMnemonic_STA, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x8d, kCpuMnemonic_STA, kCpuAddrModeAbsolute, true);
	AddMnemonic(0x9d, kCpuMnemonic_STA, kCpuAddrModeAbsoluteX, true);
	AddMnemonic(0x99, kCpuMnemonic_STA, kCpuAddrModeAbsoluteY, true);
	AddMnemonic(0x81, kCpuMnemonic_STA, kCpuAddrModeIndirectX, true);
	AddMnemonic(0x91, kCpuMnemonic_STA, kCpuAddrModeIndirectY, true);

	// STX
	AddMnemonic(0x86, kCpuMnemonic_STX, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x96, kCpuMnemonic_STX, kCpuAddrModeZeroPageY, true);
	AddMnemonic(0x8e, kCpuMnemonic_STX, kCpuAddrModeAbsolute, true);

	// STY
	AddMnemonic(0x84, kCpuMnemonic_STY, kCpuAddrModeZeroPage, true);
	AddMnemonic(0x94, kCpuMnemonic_STY, kCpuAddrModeZeroPageX, true);
	AddMnemonic(0x8c, kCpuMnemonic_STY, kCpuAddrModeAbsolute, true);

	// TAX
	AddMnemonic(0xaa, kCpuMnemonic_TAX, kCpuAddrModeImplied, true);

	// TAY
	AddMnemonic(0xa8, kCpuMnemonic_TAX, kCpuAddrModeImplied, true);

	// TSX
	AddMnemonic(0xba, kCpuMnemonic_TSX, kCpuAddrModeImplied, true);

	// TXA
	AddMnemonic(0x8a, kCpuMnemonic_TXA, kCpuAddrModeImplied, true);

	// TXS
	AddMnemonic(0x9a, kCpuMnemonic_TXS, kCpuAddrModeImplied, true);

	// TYA
	AddMnemonic(0x98, kCpuMnemonic_TYA, kCpuAddrModeImplied, true);

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
			if(mnemonics[operand].valid)
			{
				printf(" %s |", mnemonicStrings[mnemonics[operand].mnemonic].c_str());
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
			if(mnemonics[operand].valid)
			{
				printf("%s|", addrModeStrings[mnemonics[operand].addrMode].c_str());
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

bool Cpu6502::IsOpcodeValid(uint8_t operand)
{
	return mnemonics[operand].valid;
}

void Cpu6502::DumpDisassembly(uint16_t address, uint16_t size)
{
	printf("Disassembling\n");
	uint16_t pc = address;
	while(pc < address + size)
	{
		uint8_t opcode = memory_Read(pc);
		if(mnemonics[opcode].pfDisasm != 0)
		{
			pc += (mnemonics[opcode].pfDisasm)(pc,mnemonicStrings[mnemonics[opcode].mnemonic].c_str());
		}
		else
		{
			pc++;
		}
	}
}
