// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <cstdint>
#include <string>
#include "errorcodes.h"
#include "interfaces/iconfigserialisation.h"

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
	
	void AddStateSerialiser(IConfigSerialisation* serialiser);

	void SerialiseAppConfig();
	void DeserialiseAppConfig();
	
private:
	std::string loadROMFilename;
	uint16_t	loadROMAddress;
	
	json	appConfig;

	std::vector<IConfigSerialisation*> serialisers;
};

