#pragma once
#include "errorcodes.h"

extern void config_Init(void);
extern void config_Destroy(void);

extern errorcode_t config_ParseCommandLine(int32_t argc, char* argv[]);
 
extern char* config_GetLoadFilename(void);

