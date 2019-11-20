// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include "command.h"
#include "../errorcodes.h"

class WindowBase;

class WindowManager : public ICommandProcessor
{
public:
	WindowManager();
	virtual ~WindowManager();
	
	eErrorCode AddWindow(WindowBase* pWindow, std::string name);
	
	void Init();
	void Draw();
private:
	
	// ICommandProcessor
	virtual bool HandleCommand(const Command& command);
	// ~ICommandProcessor
	
	bool initialised;
	std::vector<WindowBase*> windows;
};
