// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <set>

#include "../shared_cpp/command.h"
#include "../config.h"

// setting these as defines rather than static consts because the test harness
// doesn't like static consts in its REQUIRE() statements...
#define k6502TicksADCimm 2
#define k6502TicksADCzp 3
#define k6502TicksADCzpx 4
#define k6502TicksADCabs 4
#define k6502TicksADCabsx 4
#define k6502TicksADCabsy 4
#define k6502TicksADCindx 6
#define k6502TicksADCindy 5

#define k6502TicksANDimm 2
#define k6502TicksANDzp 3
#define k6502TicksANDzpx 4
#define k6502TicksANDabs 4
#define k6502TicksANDabsx 4
#define k6502TicksANDabsy 4
#define k6502TicksANDindx 6
#define k6502TicksANDindy 5

#define k6502TicksASLacc 2
#define k6502TicksASLzp 5
#define k6502TicksASLzpx 6
#define k6502TicksASLabs 6
#define k6502TicksASLabsx 7

#define k6502TicksBCC 2
#define k6502TicksBCS 2
#define k6502TicksBEQ 2
#define k6502TicksBMI 2
#define k6502TicksBNE 2
#define k6502TicksBPL 2
#define k6502TicksBVC 2
#define k6502TicksBVS 2

#define k6502TicksBITzp 3
#define k6502TicksBITabs 4

#define k6502TicksBRK 7

#define k6502TicksCLC 2
#define k6502TicksCLD 2
#define k6502TicksCLI 2
#define k6502TicksCLV 2

#define k6502TicksCMPimm 2
#define k6502TicksCMPzp 3
#define k6502TicksCMPzpx 4
#define k6502TicksCMPabs 4
#define k6502TicksCMPabsx 4
#define k6502TicksCMPabsy 4
#define k6502TicksCMPindx 6
#define k6502TicksCMPindy 5

#define k6502TicksCPXimm 2
#define k6502TicksCPXzp 3
#define k6502TicksCPXabs 4

#define k6502TicksCPYimm 2
#define k6502TicksCPYzp 3
#define k6502TicksCPYabs 4

#define k6502TicksDECzp 5
#define k6502TicksDECzpx 6
#define k6502TicksDECabs 6
#define k6502TicksDECabsx 7

#define k6502TicksDEX 2
#define k6502TicksDEY 2

#define k6502TicksEORimm 2
#define k6502TicksEORzp 3
#define k6502TicksEORzpx 4
#define k6502TicksEORabs 4
#define k6502TicksEORabsx 4
#define k6502TicksEORabsy 4
#define k6502TicksEORindx 6
#define k6502TicksEORindy 5

#define k6502TicksINCzp 5
#define k6502TicksINCzpx 6
#define k6502TicksINCabs 6
#define k6502TicksINCabsx 7

#define k6502TicksINX 2
#define k6502TicksINY 2

#define k6502TicksJMPabs 3
#define k6502TicksJMPind 5

#define k6502TicksJSR 6

#define k6502TicksLDAimm 2
#define k6502TicksLDAzp 3
#define k6502TicksLDAzpx 4
#define k6502TicksLDAabs 4
#define k6502TicksLDAabsx 4
#define k6502TicksLDAabsy 4
#define k6502TicksLDAindx 6
#define k6502TicksLDAindy 5

#define k6502TicksLDXimm 2
#define k6502TicksLDXzp 3
#define k6502TicksLDXzpy 4
#define k6502TicksLDXabs 4
#define k6502TicksLDXabsy 4

#define k6502TicksLDYimm 2
#define k6502TicksLDYzp 3
#define k6502TicksLDYzpy 4
#define k6502TicksLDYabs 4
#define k6502TicksLDYabsy 4

#define k6502TicksLSRacc 2
#define k6502TicksLSRzp 5
#define k6502TicksLSRzpx 6
#define k6502TicksLSRabs 6
#define k6502TicksLSRabsx 7

#define k6502TicksNOP 2

#define k6502TicksORAimm 2
#define k6502TicksORAzp 3
#define k6502TicksORAzpx 4
#define k6502TicksORAabs 4
#define k6502TicksORAabsx 4
#define k6502TicksORAabsy 4
#define k6502TicksORAindx 6
#define k6502TicksORAindy 5

#define k6502TicksPHA 3
#define k6502TicksPHP 3
#define k6502TicksPLA 4
#define k6502TicksPLP 4

#define k6502TicksROLacc 2
#define k6502TicksROLzp 5
#define k6502TicksROLzpx 6
#define k6502TicksROLabs 6
#define k6502TicksROLabsx 7

#define k6502TicksRORacc 2
#define k6502TicksRORzp 5
#define k6502TicksRORzpx 6
#define k6502TicksRORabs 6
#define k6502TicksRORabsx 7

#define k6502TicksRTI 6
#define k6502TicksRTS 6

#define k6502TicksSBCimm 2
#define k6502TicksSBCzp 3
#define k6502TicksSBCzpx 4
#define k6502TicksSBCabs 4
#define k6502TicksSBCabsx 4
#define k6502TicksSBCabsy 4
#define k6502TicksSBCindx 6
#define k6502TicksSBCindy 5

#define k6502TicksSEC 2
#define k6502TicksSED 2
#define k6502TicksSEI 2

#define k6502TicksSTAzp 3
#define k6502TicksSTAzpx 4
#define k6502TicksSTAabs 4
#define k6502TicksSTAabsx 5
#define k6502TicksSTAabsy 5
#define k6502TicksSTAindx 6
#define k6502TicksSTAindy 6

#define k6502TicksSTXzp 3
#define k6502TicksSTXzpy 4
#define k6502TicksSTXabs 4

#define k6502TicksSTXzp 3
#define k6502TicksSTXzpx 4
#define k6502TicksSTXabs 4

#define k6502TicksTAX 2
#define k6502TicksTAY 2
#define k6502TicksTSX 2
#define k6502TicksTXA 2
#define k6502TicksTXS 2
#define k6502TicksTYA 2

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

		struct HistoryEntry
		{
			bool		valid;
			Registers 	reg;
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
