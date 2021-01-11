// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <map>
#include "command.h"
#include "errorcodes.h"
#include "istateserialisation.h"

class WindowBase;
class StateSerialiser;

class WindowManager : public ICommandProcessor, public IStateSerialisation
{
public:
	WindowManager();
	virtual ~WindowManager();
	
	eErrorCode AddWindow(WindowBase* pWindow, std::string name);
	
	const std::vector<std::string> GetWindows();
	
	void Init(StateSerialiser* pStateSerialiser);
	void Draw();
	bool ReceivedQuit();

	// IStateSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IStateSerialisation

private:
	
	// ICommandProcessor
	virtual bool HandleCommand(const std::shared_ptr<Command> command);
	// ~ICommandProcessor
	

	bool initialised;
	std::map<std::string,WindowBase*> windows;
	std::map<std::string,bool> windowActive;
	std::vector<std::string> windowNames;
	bool receivedQuit;
};
