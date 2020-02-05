// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <set>

#include "../shared_cpp/command.h"
#include "../config.h"

class IMemory;

class Cpu6502 : public ICommandProcessor, public IStateSerialisation
{
	public:
		static const uint8_t kNegativeSetMask = 0x80;
		static const uint8_t kNegativeClearMask = 0x7f;
		
		static const uint8_t kOverflowSetMask = 0x40;
		static const uint8_t kOverflowClearMask = 0xbf;
		
		static const uint8_t kBreakSetMask = 0x10;
		static const uint8_t kBreakClearMask = 0xef;
		
		static const uint8_t kDecimalSetMask = 0x08;
		static const uint8_t kDecimalClearMask = 0xf7;
		
		static const uint8_t kInterruptSetMask = 0x04;
		static const uint8_t kInterruptClearMask = 0xfb;
		
		static const uint8_t kZeroSetMask = 0x02;
		static const uint8_t kZeroClearMask = 0xfd;
		
		static const uint8_t kCarrySetMask = 0x01;
		static const uint8_t kCarryClearMask = 0xfe;
		
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
			kMnemonic_PHP,		kMnemonic_PLA,	kMnemonic_PLP,	kMnemonic_ROL,	kMnemonic_ROR,
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
		
		void SetMemory(IMemory* mem)
		{
			pMemory = mem;
		}

		// Register access
		
		uint16_t GetPC();
		uint8_t	GetAcc();
		uint8_t	GetX();
		uint8_t	GetY();
		uint8_t	GetStatus();
		uint8_t	GetSP();

		void SetPC(uint16_t pc);
		void SetAcc(uint8_t acc);
		void SetX(uint8_t x);
		void SetY(uint8_t y);
		void SetStatus(uint8_t status);
		void SetSP(uint8_t sp);

		/// One single clock tick
		void Tick(void);

		void ProcessInstruction(bool ignoreBreakpoints = false);		// kind of debug

		uint32_t RunToBrk();	// Runs until BRK statement. Used in unit tests.

		const Opcode* GetNextInstruction();

		struct Registers {
			uint16_t	pc;
			uint8_t		acc;
			uint8_t		x;
			uint8_t		y;
			uint8_t		status;
			uint8_t		sp;
		};

		uint16_t	next_pc;	// what the PC will be at next instruction

		// IStateSerialisation
		void SerialiseState(json& object) override;
		void DeserialiseState(json& object) override;
		// ~IStateSerialisation

		const std::set<uint16_t>&	GetBreakpoints();
		void SetBreakpoint(uint16_t addr);
		void ClearBreakpoint(uint16_t addr);
		bool IsBreakpoint(uint16_t addr);

		uint64_t GetTicksSinceBoot(){return ticksSinceBoot;}
		uint32_t GetTicksUntilExecution(){return ticksUntilExecution;}

		inline void SetZeroFlag(){reg.status |= kZeroSetMask;}
		inline void ClearZeroFlag(){reg.status &= kZeroClearMask;}
		inline bool GetZeroFlag(){return reg.status & kZeroSetMask;}

		inline void SetNegativeFlag(){reg.status |= kNegativeSetMask;}
		inline void ClearNegativeFlag(){reg.status &= kNegativeClearMask;}
		inline bool GetNegativeFlag(){return reg.status & kNegativeSetMask;}

		inline void SetDecimalFlag(){reg.status |= kDecimalSetMask;}
		inline void ClearDecimalFlag(){reg.status &= kDecimalClearMask;}
		inline bool GetDecimalFlag(){return reg.status & kDecimalSetMask;}

		inline void SetInterruptFlag(){reg.status |= kInterruptSetMask;}
		inline void ClearInterruptFlag(){reg.status &= kInterruptClearMask;}
		inline bool GetInterruptFlag(){return reg.status & kInterruptSetMask;}

		inline void SetCarryFlag(){reg.status |= kCarrySetMask;}
		inline void ClearCarryFlag(){reg.status &= kCarryClearMask;}
		inline bool GetCarryFlag(){return reg.status & kCarrySetMask;}

		inline void SetOverflowFlag(){reg.status |= kOverflowSetMask;}
		inline void ClearOverflowFlag(){reg.status &= kOverflowClearMask;}
		inline bool GetOverflowFlag(){return reg.status & kOverflowSetMask;}
		
	private:
		Registers	reg;

        void AddOpcode(uint8_t value, enum eMnemonic mnemonic, enum eAddressingMode addrMode, eMemoryOp memoryOp);
        void AddEmptyOpcode(uint8_t value);
        
		// ICommandProcessor
		bool HandleCommand(const Command &command) override;
		// ~ICommandProcessor
		
		// static setup
		Opcode	 	opcodes[256];
		std::string mnemonicStrings[kMnemonic_Num];
		std::string addrModeStrings[kAddrMode_Num];


		IMemory*	pMemory;

		std::set<uint16_t> 	breakpoints;
		uint64_t	ticksSinceBoot;
		bool		haltOnTick;
		bool		bHaltOnInstruction;
		bool		bHalted;
		int32_t	ticksUntilExecution;
};
