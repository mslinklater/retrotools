#pragma once
#include <inttypes.h>

typedef uint32_t errorcode_t;

#define ERROR_OK						0
#define ERROR_COMMAND_LINE_PARSING		1000

extern void errorcodes_printf(errorcode_t error_code);

