// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <cstdint>
#include <string>
#include "errorcodes.h"
//#include "json11/json11.hpp"
#include "json.hpp"
using json = nlohmann::json;

class IConfigSerialisation
{
public:
//	virtual void SerialiseState(json11::Json& object) = 0;
//	virtual void DeserialiseState(json11::Json& object) = 0;
};

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

