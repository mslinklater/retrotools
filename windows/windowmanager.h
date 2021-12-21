// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <map>
#include "system/commandcenter.h"
#include "system/errorcodes.h"
#include "system/istateserialisation.h"

class WindowBase;
class StateSerialiser;

class WindowManager : public ICommandHandler, public IStateSerialisation
{
public:
	WindowManager();
	virtual ~WindowManager();
	
	eErrorCode AddWindow(WindowBase* pWindow, std::string name);
	
	const std::vector<std::string> GetWindows();
	
	void Init(StateSerialiser* pStateSerialiser);
	void Draw();
	bool ReceivedQuit();

	ISERIALISATION_API

private:
	
	// ICommandHandler
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command);
	// ~ICommandHandler
	
	bool initialised;
	std::map<std::string,WindowBase*> windows;
	std::map<std::string,bool> windowActive;
	std::vector<std::string> windowNames;
	bool receivedQuit;
};
