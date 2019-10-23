#pragma once
#include <inttypes.h>
#include <stdbool.h>


extern void cpu_Init(void);
extern void cpu_DumpInfo(void);

extern void cpu_dumpDisassembly(uint16_t address, uint16_t size);
extern bool cpu_IsOperandValid(uint8_t operand);
