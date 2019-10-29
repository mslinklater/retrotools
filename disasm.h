#include <cstdint>

#include "errorcodes.h"

extern void disasm_Init(void);
extern eErrorCode disasm_Disassemble(uint16_t address, uint16_t size, uint16_t org);
extern void disasm_DumpToTTY(void);
