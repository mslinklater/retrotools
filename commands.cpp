// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commands.h"
#include "system/command.h"

void Commands::NewProject()
{
	Command cmd;
	cmd.name = kNewProjectCommand;
	CommandCenter::Instance()->Broadcast(cmd);
}
