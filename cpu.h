#pragma once
#include <string>

class Memory;

class Cpu6502
{
	public:
		enum eVariant
		{
			k6502,
			k6507
		};
		
		enum eAddressingMode 
		{
			kAddrModeAccumulator = 0,
			kAddrModeImmediate,
			kAddrModeZeroPage,
			kAddrModeZeroPageX,
			kAddrModeZeroPageY,
			kAddrModeAbsolute,
			kAddrModeAbsoluteX,
			kAddrModeAbsoluteY,
			kAddrModeIndirectX,
			kAddrModeIndirectY,
			kAddrModeRelative,
			kAddrModeImplied,
			kAddrModeIndirect,
			kAddrMode_Num
		};

		enum eMnemonic 
		{
			kMnemonic_ADC=0,	kMnemonic_AND,	kMnemonic_ASL,	kMnemonic_BCC,
			kMnemonic_BCS,		kMnemonic_BEQ,	kMnemonic_BIT,	kMnemonic_BMI,
			kMnemonic_BNE,		kMnemonic_BPL,	kMnemonic_BRK,	kMnemonic_BVC,
			kMnemonic_BVS,		kMnemonic_CLC,	kMnemonic_CLD,	kMnemonic_CLI,
			kMnemonic_CLV,		kMnemonic_CMP,	kMnemonic_CPX,	kMnemonic_CPY,
			kMnemonic_DEC,		kMnemonic_DEX,	kMnemonic_DEY,	kMnemonic_EOR,
			kMnemonic_INC,		kMnemonic_INX,	kMnemonic_INY,	kMnemonic_JMP,
			kMnemonic_JSR,		kMnemonic_LDA,	kMnemonic_LDX,	kMnemonic_LDY,
			kMnemonic_LSR,		kMnemonic_NOP,	kMnemonic_ORA,	kMnemonic_PHA,
			kMnemonic_PHP,		kMnemonic_PLA,	kMnemonic_ROL,	kMnemonic_ROR,
			kMnemonic_RTI,		kMnemonic_RTS,	kMnemonic_SBC,	kMnemonic_SEC,
			kMnemonic_SED,		kMnemonic_SEI,	kMnemonic_STA,	kMnemonic_STX,
			kMnemonic_STY,		kMnemonic_TAX,	kMnemonic_TAY,	kMnemonic_TSX,
			kMnemonic_TXA,		kMnemonic_TXS,	kMnemonic_TYA,	kMnemonic_Num
		};

		struct Opcode {
			bool					valid;
			uint8_t					value;
			uint8_t					length;
			enum eMnemonic			mnemonic;
			enum eAddressingMode	addrMode;
			bool					undocumented;
//			uint8_t (*pfDisasm)(uint16_t,const char*);	// To be deprecatd
		};

		Cpu6502();
		virtual ~Cpu6502(){}
		
		void Init(eVariant variant);
		void DumpInfo();
		const std::string& GetMnemonicString(eMnemonic mnemonic);
		
		const Opcode* GetOpcode(uint16_t opcode) const;
		
		void SetMemory(Memory* mem)
		{
			pMemory = mem;
		}
		
	private:
		uint16_t	pc;
		uint8_t		acc;
		uint8_t		x;
		uint8_t		y;
		uint8_t		status;
		uint8_t		sp;

        void AddOpcode(uint8_t value, enum eMnemonic mnemonic, enum eAddressingMode addrMode, bool documented);
        void AddEmptyOpcode(uint8_t value);
        
		// static setup
        bool 		staticsInitialised;
		Opcode	 	opcodes[256];
		std::string 	mnemonicStrings[kMnemonic_Num];
		std::string 	addrModeStrings[kAddrMode_Num];

		Memory*	pMemory;
};
