// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include "command.h"

class WindowBase;

class WindowManager : public ICommandProcessor
{
public:
	WindowManager();
	virtual ~WindowManager();
	
	void Init();
	void Draw();
private:
	std::vector<WindowBase*> windows;
};
