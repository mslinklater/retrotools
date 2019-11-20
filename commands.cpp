// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commands.h"
#include "system/command.h"
#include "log.h"

void Commands::NewProject()
{
	LOGINFO("Commands::NewProject");
	Command cmd;
	cmd.name = kNewProjectCommand;
	CommandCenter::Instance()->Broadcast(cmd);
}

void Commands::ToggleWindow(std::string windowName)
{
	LOGINFOF("Commands::ToggleWindow %s", windowName.c_str());
	Command cmd;
	cmd.name = kToggleWindowCommand;
	cmd.payload = windowName;
	CommandCenter::Instance()->Broadcast(cmd);
}

