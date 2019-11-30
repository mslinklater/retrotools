// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <map>
#include "command.h"
#include "../errorcodes.h"
#include "../config.h"

class WindowBase;

class WindowManager : public ICommandProcessor, public IConfigSerialisation
{
public:
	WindowManager();
	virtual ~WindowManager();
	
	eErrorCode AddWindow(WindowBase* pWindow, std::string name);
	
	const std::vector<std::string> GetWindows();
	
	void Init();
	void Draw();
	bool ReceivedQuit();
private:
	
	// ICommandProcessor
	virtual bool HandleCommand(const Command& command);
	// ~ICommandProcessor
	
	// IConfigSerialisation
	virtual void SerialiseState(json11::Json object);
	virtual void DeserialiseState(json11::Json object);
	// ~IConfigSerialisation

	bool initialised;
	std::map<std::string,WindowBase*> windows;
	std::map<std::string,bool> windowActive;
	std::vector<std::string> windowNames;
	bool receivedQuit;
};
