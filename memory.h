#pragma once

#include "errorcodes.h"

#define MEMORY_SIZE 65536

extern errorcode_t memory_Init(void);
extern errorcode_t memory_Destroy(void);

extern void memory_Write(uint16_t address, uint8_t val);
extern uint8_t memory_Read(uint16_t address);
