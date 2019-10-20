#pragma once
#include <inttypes.h>
#include <stdbool.h>

#define kCpuAddrModeImmediate	1
#define kCpuAddrModeZeroPage	2
#define kCpuAddrModeZeroPageX	3
#define kCpuAddrModeAbsolute	4
#define kCpuAddrModeAbsoluteX	5
#define kCpuAddrModeAbsoluteY	6
#define kCpuAddrModeIndirectX	7
#define kCpuAddrModeIndirectY	8

#define kCpuMnemonic_ADC		1
#define kCpuMnemonic_AND		2
#define kCpuMnemonic_ASL		3
#define kCpuMnemonic_BCC		4
#define kCpuMnemonic_BCS		5
#define kCpuMnemonic_BEQ		6
#define kCpuMnemonic_BIT		7
#define kCpuMnemonic_BMI		8
#define kCpuMnemonic_BNE		9
#define kCpuMnemonic_BPL		10
#define kCpuMnemonic_BRK		11
#define kCpuMnemonic_BVC		12
#define kCpuMnemonic_BVS		13
#define kCpuMnemonic_CLC		14
#define kCpuMnemonic_CLD		15
#define kCpuMnemonic_CLI		16
#define kCpuMnemonic_CLV		17
#define kCpuMnemonic_CMP		18
#define kCpuMnemonic_CPX		19
#define kCpuMnemonic_CPY		20
#define kCpuMnemonic_DEC		21
#define kCpuMnemonic_DEX		22
#define kCpuMnemonic_DEY		23
#define kCpuMnemonic_EOR		24
#define kCpuMnemonic_INC		25
#define kCpuMnemonic_INX		26
#define kCpuMnemonic_INY		27
#define kCpuMnemonic_JMP		28
#define kCpuMnemonic_JSR		29
#define kCpuMnemonic_LDA		30
#define kCpuMnemonic_LDX		31
#define kCpuMnemonic_LDY		32
#define kCpuMnemonic_LSR		33
#define kCpuMnemonic_NOP		34
#define kCpuMnemonic_ORA		35
#define kCpuMnemonic_PHA		36
#define kCpuMnemonic_PHP		37
#define kCpuMnemonic_PLA		38
#define kCpuMnemonic_ROL		39
#define kCpuMnemonic_ROR		40
#define kCpuMnemonic_RTI		41
#define kCpuMnemonic_RTS		42
#define kCpuMnemonic_SBC		43
#define kCpuMnemonic_SEC		44
#define kCpuMnemonic_SED		45
#define kCpuMnemonic_SEI		46
#define kCpuMnemonic_STA		47
#define kCpuMnemonic_STX		48
#define kCpuMnemonic_STY		49
#define kCpuMnemonic_TAX		50
#define kCpuMnemonic_TAY		51
#define kCpuMnemonic_TSX		52
#define kCpuMnemonic_TXA		53
#define kCpuMnemonic_TXS		54
#define kCpuMnemonic_TYA		55
#define kCpuNumMnemonics		56

extern void cpu_Init(void);
extern void cpu_DumpInfo(void);

extern void cpu_dumpDisassembly(uint16_t address, uint16_t size);

