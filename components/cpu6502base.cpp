#include "cpu6502base.h"
#include "../shared_cpp/log.h"

Cpu6502Base::Cpu6502Base()
{
	LOGINFO("Cpu6502Base::Constructor");

	cyclesSinceBoot = 0;
	haltOnTick = false;

	pAddressBus = nullptr;
	pDataBus = nullptr;
	pSyncPin = nullptr;
	pVssPin = nullptr;
	pRdyPin = nullptr;
	pTimer0Pin = nullptr;
	pTimer1Pin = nullptr;
	pTimer2Pin = nullptr;
	pResPin = nullptr;
	pSoPin = nullptr;
	pIrqPin = nullptr;
	pNmiPin = nullptr;
	pVccPin = nullptr;
	pReadWritePin = nullptr;

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
	AddOpcode(0x69, kMnemonic_ADC, kAddrModeImmediate, kNone);
	AddOpcode(0x65, kMnemonic_ADC, kAddrModeZeroPage, kRead);
	AddOpcode(0x75, kMnemonic_ADC, kAddrModeZeroPageX, kRead);
	AddOpcode(0x6d, kMnemonic_ADC, kAddrModeAbsolute, kRead);
	AddOpcode(0x7d, kMnemonic_ADC, kAddrModeAbsoluteX, kRead);
	AddOpcode(0x79, kMnemonic_ADC, kAddrModeAbsoluteY, kRead);
	AddOpcode(0x61, kMnemonic_ADC, kAddrModeIndirectX, kRead);
	AddOpcode(0x71, kMnemonic_ADC, kAddrModeIndirectY, kRead);

	// AND
	AddOpcode(0x29, kMnemonic_AND, kAddrModeImmediate, kNone);
	AddOpcode(0x25, kMnemonic_AND, kAddrModeZeroPage, kRead);
	AddOpcode(0x35, kMnemonic_AND, kAddrModeZeroPageX, kRead);
	AddOpcode(0x2d, kMnemonic_AND, kAddrModeAbsolute, kRead);
	AddOpcode(0x3d, kMnemonic_AND, kAddrModeAbsoluteX, kRead);
	AddOpcode(0x39, kMnemonic_AND, kAddrModeAbsoluteY, kRead);
	AddOpcode(0x21, kMnemonic_AND, kAddrModeIndirectX, kRead);
	AddOpcode(0x31, kMnemonic_AND, kAddrModeIndirectY, kRead);

	// ASL
	AddOpcode(0x0a, kMnemonic_ASL, kAddrModeAccumulator, kNone);
	AddOpcode(0x06, kMnemonic_ASL, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0x16, kMnemonic_ASL, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0x0e, kMnemonic_ASL, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0x1e, kMnemonic_ASL, kAddrModeAbsoluteX, kReadWrite);

	//BCC
	AddOpcode(0x90, kMnemonic_BCC, kAddrModeRelative, kNone);

	// BCS
	AddOpcode(0xb0, kMnemonic_BCS, kAddrModeRelative, kNone);

	// BEQ
	AddOpcode(0xf0, kMnemonic_BEQ, kAddrModeRelative, kNone);
	
	// BIT
	AddOpcode(0x24, kMnemonic_BIT, kAddrModeZeroPage, kRead);
	AddOpcode(0x2c, kMnemonic_BIT, kAddrModeAbsolute, kNone);

	// BMI
	AddOpcode(0x30, kMnemonic_BMI, kAddrModeRelative, kNone);

	// BNE
	AddOpcode(0xd0, kMnemonic_BNE, kAddrModeRelative, kNone);

	// BPL
	AddOpcode(0x10, kMnemonic_BPL, kAddrModeRelative, kNone);

	// BRK
	AddOpcode(0x00, kMnemonic_BRK, kAddrModeImplied, kNone);

	// BVC
	AddOpcode(0x50, kMnemonic_BVC, kAddrModeRelative, kNone);

	//BVS
	AddOpcode(0x70, kMnemonic_BVS, kAddrModeRelative, kNone);

	// CLC
	AddOpcode(0x18, kMnemonic_CLC, kAddrModeImplied, kNone);

	// CLD
	AddOpcode(0xd8, kMnemonic_CLD, kAddrModeImplied, kNone);

	// CLI
	AddOpcode(0x58, kMnemonic_CLI, kAddrModeImplied, kNone);

	// CLV
	AddOpcode(0xb8, kMnemonic_CLV, kAddrModeImplied, kNone);

	// CMP
	AddOpcode(0xc9, kMnemonic_CMP, kAddrModeImmediate, kNone);
	AddOpcode(0xc5, kMnemonic_CMP, kAddrModeZeroPage, kRead);
	AddOpcode(0xd5, kMnemonic_CMP, kAddrModeZeroPageX, kRead);
	AddOpcode(0xcd, kMnemonic_CMP, kAddrModeAbsolute, kRead);
	AddOpcode(0xdd, kMnemonic_CMP, kAddrModeAbsoluteX, kRead);
	AddOpcode(0xd9, kMnemonic_CMP, kAddrModeAbsoluteY, kRead);
	AddOpcode(0xc1, kMnemonic_CMP, kAddrModeIndirectX, kRead);
	AddOpcode(0xd1, kMnemonic_CMP, kAddrModeIndirectY, kRead);

	// CPX
	AddOpcode(0xe0, kMnemonic_CPX, kAddrModeImmediate, kNone);
	AddOpcode(0xe4, kMnemonic_CPX, kAddrModeZeroPage, kRead);
	AddOpcode(0xec, kMnemonic_CPX, kAddrModeAbsolute, kRead);
	
	// CPY
	AddOpcode(0xc0, kMnemonic_CPY, kAddrModeImmediate, kNone);
	AddOpcode(0xc4, kMnemonic_CPY, kAddrModeZeroPage, kRead);
	AddOpcode(0xcc, kMnemonic_CPY, kAddrModeAbsolute, kRead);

	// DEC
	AddOpcode(0xc6, kMnemonic_DEC, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0xd6, kMnemonic_DEC, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0xce, kMnemonic_DEC, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0xde, kMnemonic_DEC, kAddrModeAbsoluteX, kReadWrite);

	// DEX
	AddOpcode(0xca, kMnemonic_DEX, kAddrModeImplied, kNone);

	// DEY
	AddOpcode(0x88, kMnemonic_DEY, kAddrModeImplied, kNone);

	// EOR
	AddOpcode(0x49, kMnemonic_EOR, kAddrModeImmediate, kNone);
	AddOpcode(0x45, kMnemonic_EOR, kAddrModeZeroPage, kRead);
	AddOpcode(0x55, kMnemonic_EOR, kAddrModeZeroPageX, kRead);
	AddOpcode(0x4d, kMnemonic_EOR, kAddrModeAbsolute, kRead);
	AddOpcode(0x5d, kMnemonic_EOR, kAddrModeAbsoluteX, kRead);
	AddOpcode(0x59, kMnemonic_EOR, kAddrModeAbsoluteY, kRead);
	AddOpcode(0x41, kMnemonic_EOR, kAddrModeIndirectX, kRead);
	AddOpcode(0x51, kMnemonic_EOR, kAddrModeIndirectY, kRead);

	// INC
	AddOpcode(0xe6, kMnemonic_INC, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0xf6, kMnemonic_INC, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0xee, kMnemonic_INC, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0xfe, kMnemonic_INC, kAddrModeAbsoluteX, kReadWrite);

	// INX
	AddOpcode(0xe8, kMnemonic_INX, kAddrModeImplied, kNone);

	// INY
	AddOpcode(0xc8, kMnemonic_INY, kAddrModeImplied, kNone);

	// JMP
	AddOpcode(0x4c, kMnemonic_JMP, kAddrModeAbsolute, kNone);
	AddOpcode(0x2c, kMnemonic_JMP, kAddrModeIndirect, kRead);

	// JSR
	AddOpcode(0x20, kMnemonic_JSR, kAddrModeAbsolute, kNone);

	// LDA
	AddOpcode(0xa9, kMnemonic_LDA, kAddrModeImmediate, kNone);
	AddOpcode(0xa5, kMnemonic_LDA, kAddrModeZeroPage, kRead);
	AddOpcode(0xb5, kMnemonic_LDA, kAddrModeZeroPageX, kRead);
	AddOpcode(0xad, kMnemonic_LDA, kAddrModeAbsolute, kRead);
	AddOpcode(0xbd, kMnemonic_LDA, kAddrModeAbsoluteX, kRead);
	AddOpcode(0xb9, kMnemonic_LDA, kAddrModeAbsoluteY, kRead);
	AddOpcode(0xa1, kMnemonic_LDA, kAddrModeIndirectX, kRead);
	AddOpcode(0xb1, kMnemonic_LDA, kAddrModeIndirectY, kRead);

	// LDX
	AddOpcode(0xa2, kMnemonic_LDX, kAddrModeImmediate, kNone);
	AddOpcode(0xa6, kMnemonic_LDX, kAddrModeZeroPage, kRead);
	AddOpcode(0xb6, kMnemonic_LDX, kAddrModeZeroPageY, kRead);
	AddOpcode(0xae, kMnemonic_LDX, kAddrModeAbsolute, kRead);
	AddOpcode(0xbe, kMnemonic_LDX, kAddrModeAbsoluteY, kRead);

	// LDY
	AddOpcode(0xa0, kMnemonic_LDY, kAddrModeImmediate, kNone);
	AddOpcode(0xa4, kMnemonic_LDY, kAddrModeZeroPage, kRead);
	AddOpcode(0xb4, kMnemonic_LDY, kAddrModeZeroPageX, kRead);
	AddOpcode(0xac, kMnemonic_LDY, kAddrModeAbsolute, kRead);
	AddOpcode(0xbc, kMnemonic_LDY, kAddrModeAbsoluteX, kRead);

	// LSR
	AddOpcode(0x4a, kMnemonic_LSR, kAddrModeAccumulator, kNone);
	AddOpcode(0x46, kMnemonic_LSR, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0x56, kMnemonic_LSR, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0x4e, kMnemonic_LSR, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0x5e, kMnemonic_LSR, kAddrModeAbsoluteX, kReadWrite);

	// NOP
	AddOpcode(0xea, kMnemonic_NOP, kAddrModeImplied, kNone);

	// ORA
	AddOpcode(0x09, kMnemonic_ORA, kAddrModeImmediate, kNone);
	AddOpcode(0x05, kMnemonic_ORA, kAddrModeZeroPage, kRead);
	AddOpcode(0x15, kMnemonic_ORA, kAddrModeZeroPageX, kRead);
	AddOpcode(0x0d, kMnemonic_ORA, kAddrModeAbsolute, kRead);
	AddOpcode(0x1d, kMnemonic_ORA, kAddrModeAbsoluteX, kRead);
	AddOpcode(0x19, kMnemonic_ORA, kAddrModeAbsoluteY, kRead);
	AddOpcode(0x01, kMnemonic_ORA, kAddrModeIndirectX, kRead);
	AddOpcode(0x11, kMnemonic_ORA, kAddrModeIndirectY, kRead);

	// PHA
	AddOpcode(0x48, kMnemonic_PHA, kAddrModeImplied, kWrite);

	// PHP
	AddOpcode(0x08, kMnemonic_PHP, kAddrModeImplied, kWrite);

	// PLA
	AddOpcode(0x68, kMnemonic_PLA, kAddrModeImplied, kRead);

	// PLA
	AddOpcode(0x28, kMnemonic_PLP, kAddrModeImplied, kRead);

	// ROL
	AddOpcode(0x2a, kMnemonic_ROL, kAddrModeAccumulator, kNone);
	AddOpcode(0x26, kMnemonic_ROL, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0x36, kMnemonic_ROL, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0x2e, kMnemonic_ROL, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0x3e, kMnemonic_ROL, kAddrModeAbsoluteX, kReadWrite);

	// ROR
	AddOpcode(0x6a, kMnemonic_ROR, kAddrModeAccumulator, kNone);
	AddOpcode(0x66, kMnemonic_ROR, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0x76, kMnemonic_ROR, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0x6e, kMnemonic_ROR, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0x7e, kMnemonic_ROR, kAddrModeAbsoluteX, kReadWrite);

	// RTI
	AddOpcode(0x40, kMnemonic_RTI, kAddrModeImplied, kNone);

	// RTS
	AddOpcode(0x60, kMnemonic_RTS, kAddrModeImplied, kNone);

	// SBC
	AddOpcode(0xe9, kMnemonic_SBC, kAddrModeImmediate, kNone);
	AddOpcode(0xe5, kMnemonic_SBC, kAddrModeZeroPage, kReadWrite);
	AddOpcode(0xf5, kMnemonic_SBC, kAddrModeZeroPageX, kReadWrite);
	AddOpcode(0xed, kMnemonic_SBC, kAddrModeAbsolute, kReadWrite);
	AddOpcode(0xfd, kMnemonic_SBC, kAddrModeAbsoluteX, kReadWrite);
	AddOpcode(0xf9, kMnemonic_SBC, kAddrModeAbsoluteY, kReadWrite);
	AddOpcode(0xe1, kMnemonic_SBC, kAddrModeIndirectX, kReadWrite);
	AddOpcode(0xf1, kMnemonic_SBC, kAddrModeIndirectY, kReadWrite);

	// SEC
	AddOpcode(0x38, kMnemonic_SEC, kAddrModeImplied, kNone);

	// SED
	AddOpcode(0xf8, kMnemonic_SED, kAddrModeImplied, kNone);

	// SEI
	AddOpcode(0x78, kMnemonic_SEI, kAddrModeImplied, kNone);

	// STA
	AddOpcode(0x85, kMnemonic_STA, kAddrModeZeroPage, kWrite);
	AddOpcode(0x95, kMnemonic_STA, kAddrModeZeroPageX, kWrite);
	AddOpcode(0x8d, kMnemonic_STA, kAddrModeAbsolute, kWrite);
	AddOpcode(0x9d, kMnemonic_STA, kAddrModeAbsoluteX, kWrite);
	AddOpcode(0x99, kMnemonic_STA, kAddrModeAbsoluteY, kWrite);
	AddOpcode(0x81, kMnemonic_STA, kAddrModeIndirectX, kWrite);
	AddOpcode(0x91, kMnemonic_STA, kAddrModeIndirectY, kWrite);

	// STX
	AddOpcode(0x86, kMnemonic_STX, kAddrModeZeroPage, kWrite);
	AddOpcode(0x96, kMnemonic_STX, kAddrModeZeroPageY, kWrite);
	AddOpcode(0x8e, kMnemonic_STX, kAddrModeAbsolute, kWrite);

	// STY
	AddOpcode(0x84, kMnemonic_STY, kAddrModeZeroPage, kWrite);
	AddOpcode(0x94, kMnemonic_STY, kAddrModeZeroPageX, kWrite);
	AddOpcode(0x8c, kMnemonic_STY, kAddrModeAbsolute, kWrite);

	// TAX
	AddOpcode(0xaa, kMnemonic_TAX, kAddrModeImplied, kNone);

	// TAY
	AddOpcode(0xa8, kMnemonic_TAY, kAddrModeImplied, kNone);

	// TSX
	AddOpcode(0xba, kMnemonic_TSX, kAddrModeImplied, kNone);

	// TXA
	AddOpcode(0x8a, kMnemonic_TXA, kAddrModeImplied, kNone);

	// TXS
	AddOpcode(0x9a, kMnemonic_TXS, kAddrModeImplied, kNone);

	// TYA
	AddOpcode(0x98, kMnemonic_TYA, kAddrModeImplied, kNone);

	// Illegal opcodes
	AddOpcode(0x04, kMnemonic_NOP, kAddrModeZeroPage, kNone);
}

Cpu6502Base::~Cpu6502Base()
{
	LOGINFO("Cpu6502Base::Destructor");
}

void Cpu6502Base::Init(EVariant variant)
{
}

void Cpu6502Base::AddOpcode(uint8_t value, enum EMnemonic mnemonic, enum EAddressingMode addrMode, EMemoryOp memoryOp)
{
	opcodes[value].value = value;
	opcodes[value].valid = true;
	opcodes[value].mnemonic = mnemonic;
	opcodes[value].addrMode = addrMode;
	opcodes[value].memoryOp = memoryOp;

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
			opcodes[value].length = 3;
			break;
		default:
			break;
	}
}

void Cpu6502Base::AddEmptyOpcode(uint8_t value)
{
	opcodes[value].valid = false;
}

const Cpu6502Base::Opcode* Cpu6502Base::GetOpcode(uint16_t opcode) const
{
	return &opcodes[opcode];
}

const std::string& Cpu6502Base::GetMnemonicString(EMnemonic mnemonic) const
{
	return mnemonicStrings[mnemonic];
}

const std::set<uint16_t>&	Cpu6502Base::GetBreakpoints()
{
	return breakpoints;
}

void Cpu6502Base::SetBreakpoint(uint16_t addr)
{
	breakpoints.insert(addr);
}

void Cpu6502Base::ClearBreakpoint(uint16_t addr)
{
	breakpoints.erase(addr);
}

bool Cpu6502Base::IsBreakpoint(uint16_t addr)
{
	return breakpoints.find(addr) != breakpoints.end();
}

uint32_t Cpu6502Base::RunToBrk()
{
	uint32_t ticks = 0;

	EMnemonic mnemonic = GetExecuteOpcode()->mnemonic;

	while(mnemonic != EMnemonic::kMnemonic_BRK)
	{
		Tick(false);
		Tick(true);
		ticks++;
		mnemonic = GetExecuteOpcode()->mnemonic;
	}

	return ticks-1; // sub 1 for the BRK decode.
}
