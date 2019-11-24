// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>

class Memory;

class Cpu6502
{
	public:
		static const uint8_t kNegativeFlagMask = 0x80;
		static const uint8_t kOverflowFlagMask = 0x40;
		static const uint8_t kBreakFlagMask = 0x10;
		static const uint8_t kDecimalFlagMask = 0x08;
		static const uint8_t kInterruptFlagMask = 0x04;
		static const uint8_t kZeroFlagMask = 0x02;
		static const uint8_t kCarryFlagMask = 0x01;
		
		enum eVariant
		{
			k6502,
			k6507
		};
		
		enum eMemoryOp
		{
			kRead,
			kWrite,
			kReadWrite,
			kNone
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
			bool			valid;
			uint8_t			value;
			uint8_t			length;
			eMnemonic		mnemonic;
			eAddressingMode	addrMode;
			eMemoryOp		memoryOp;
		};

		Cpu6502();
		virtual ~Cpu6502(){}
		
		void Init(eVariant variant);
		void DumpInfo();
		const std::string& GetMnemonicString(eMnemonic mnemonic) const;
		
		const Opcode* GetOpcode(uint16_t opcode) const;
		
		void SetMemory(Memory* mem)
		{
			pMemory = mem;
		}

		// Register access
		
		uint16_t GetPC();
		void SetPC(uint16_t pc);

		uint8_t	GetAcc();
		void SetAcc(uint8_t acc);

		uint8_t	GetX();
		void SetX(uint8_t x);

		uint8_t	GetY();
		void SetY(uint8_t y);

		uint8_t	GetStatus();
		void SetStatus(uint8_t status);

		uint8_t	GetSP();
		void SetSP(uint8_t sp);
		
	private:
		uint16_t	reg_pc;
		uint8_t		reg_acc;
		uint8_t		reg_x;
		uint8_t		reg_y;
		uint8_t		reg_status;
		uint8_t		reg_sp;

        void AddOpcode(uint8_t value, enum eMnemonic mnemonic, enum eAddressingMode addrMode, eMemoryOp memoryOp);
        void AddEmptyOpcode(uint8_t value);
        
		// static setup
        bool 		staticsInitialised;
		Opcode	 	opcodes[256];
		std::string mnemonicStrings[kMnemonic_Num];
		std::string addrModeStrings[kAddrMode_Num];

		Memory*	pMemory;
};
