#pragma once
#include <cstdint>
#include <string>
#include "errorcodes.h"

class Config
{
public:
    Config();
    virtual ~Config(){}
    
    void Init();
    void Destroy();
	
	eErrorCode 	ParseCommandLine(int32_t argc, char* argv[]);
	std::string GetLoadFilename();
	uint16_t 	GetLoadAddress();
private:
	std::string loadROMFilename;
	uint16_t	loadROMAddress;
};

