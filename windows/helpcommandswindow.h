// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "windowmanager.h"
#include "windowbase.h"
#include "system/usercommands.h"

class HelpCommandsWindow : public WindowBase
{
public:
	
	HelpCommandsWindow();
	virtual ~HelpCommandsWindow();
	
	virtual void Draw(void);
		
private:
	const std::vector<UserCommands::CommandInfo> userCommands;
	void DrawCommandsForType(UserCommands::Type t);
};
