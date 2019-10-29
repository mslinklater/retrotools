#pragma once
#include <string>
//#include <inttypes.h>

class Cpu6502
{
	public:
		enum eAddressingMode 
		{
			kCpuAddrModeAccumulator = 0,
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
			kCpuAddrModeIndirect,
			kCpuAddrMode_Num
		};

		enum eMnemonic 
		{
			kCpuMnemonic_ADC=0,	kCpuMnemonic_AND,	kCpuMnemonic_ASL,	kCpuMnemonic_BCC,
			kCpuMnemonic_BCS,	kCpuMnemonic_BEQ,	kCpuMnemonic_BIT,	kCpuMnemonic_BMI,
			kCpuMnemonic_BNE,	kCpuMnemonic_BPL,	kCpuMnemonic_BRK,	kCpuMnemonic_BVC,
			kCpuMnemonic_BVS,	kCpuMnemonic_CLC,	kCpuMnemonic_CLD,	kCpuMnemonic_CLI,
			kCpuMnemonic_CLV,	kCpuMnemonic_CMP,	kCpuMnemonic_CPX,	kCpuMnemonic_CPY,
			kCpuMnemonic_DEC,	kCpuMnemonic_DEX,	kCpuMnemonic_DEY,	kCpuMnemonic_EOR,
			kCpuMnemonic_INC,	kCpuMnemonic_INX,	kCpuMnemonic_INY,	kCpuMnemonic_JMP,
			kCpuMnemonic_JSR,	kCpuMnemonic_LDA,	kCpuMnemonic_LDX,	kCpuMnemonic_LDY,
			kCpuMnemonic_LSR,	kCpuMnemonic_NOP,	kCpuMnemonic_ORA,	kCpuMnemonic_PHA,
			kCpuMnemonic_PHP,	kCpuMnemonic_PLA,	kCpuMnemonic_ROL,	kCpuMnemonic_ROR,
			kCpuMnemonic_RTI,	kCpuMnemonic_RTS,	kCpuMnemonic_SBC,	kCpuMnemonic_SEC,
			kCpuMnemonic_SED,	kCpuMnemonic_SEI,	kCpuMnemonic_STA,	kCpuMnemonic_STX,
			kCpuMnemonic_STY,	kCpuMnemonic_TAX,	kCpuMnemonic_TAY,	kCpuMnemonic_TSX,
			kCpuMnemonic_TXA,	kCpuMnemonic_TXS,	kCpuMnemonic_TYA,	kCpuMnemonic_Num
		};

		struct Mnemonic {
			bool					valid;
			uint8_t					value;
			enum eMnemonic			mnemonic;
			enum eAddressingMode	addrMode;
			bool					undocumented;
			uint8_t (*pfDisasm)(uint16_t,const char*);	// To be deprecatd
		};

		Cpu6502();
		virtual ~Cpu6502(){}
		
		void Init();
		void DumpInfo();
		void DumpDisassembly(uint16_t address, uint16_t size);
		bool IsOpcodeValid(uint8_t operand);
	private:
		uint16_t	pc;
		uint8_t		acc;
		uint8_t		x;
		uint8_t		y;
		uint8_t		status;
		uint8_t		sp;

        void AddMnemonic(uint8_t value, enum eMnemonic mnemonic, enum eAddressingMode addrMode, bool documented);
        void AddEmptyMnemonic(uint8_t value);
        
		// static setup
        static bool 		staticsInitialised;
		static Mnemonic 	mnemonics[256];
		static std::string 	mnemonicStrings[kCpuMnemonic_Num];
		static std::string 	addrModeStrings[kCpuAddrMode_Num];
};
