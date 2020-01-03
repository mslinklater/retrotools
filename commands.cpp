// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commands.h"
#include "shared_cpp/command.h"
#include "shared_cpp/log.h"

void Commands::NewProject()
{
	LOGINFO("Commands::NewProject");
	Command cmd;
	cmd.name = kNewProjectCommand;
	CommandCenter::Instance()->Broadcast(cmd);
}


void Commands::Halt(bool halt, std::string runType)
{
	LOGINFOF("Commands::Halt %s", halt ? "true" : "false");
	Command cmd;
	cmd.name = kHaltCommand;
	cmd.payload = halt ? "true" : "false";
	cmd.payload2 = runType;
	CommandCenter::Instance()->Broadcast(cmd);
}
