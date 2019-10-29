#pragma once
//#include <inttypes.h>

//typedef uint32_t errorcode_t;

//#define ERROR_OK							0

//#define ERROR_COMMAND_LINE_PARSING			1000

//#define ERROR_MEMORY_ALREADY_INITIALISED	2000
//#define ERROR_MEMORY_NOT_INITIALISED		2001

//#define ERROR_FILE_NOT_FOUND				3000

//extern void errorcodes_printf(errorcode_t error_code);

enum eErrorCode {
	kError_OK = 0,
	kError_CommandLineParsing,
	kError_MemoryAlreadyInitialised,
	kError_MemoryNotInitialised,
	kError_FileNotFound
};
