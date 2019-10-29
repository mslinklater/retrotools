#pragma once
#include <cstdint>
#include <string>
#include "errorcodes.h"

extern void config_Init(void);
extern void config_Destroy(void);

extern eErrorCode config_ParseCommandLine(int32_t argc, char* argv[]);
 
extern std::string config_GetLoadFilename(void);
extern uint16_t config_GetLoadAddress(void);

