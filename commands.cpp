// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commands.h"
#include "shared_cpp/command.h"
#include "shared_cpp/log.h"

void Commands::Halt(bool halt, std::string runType, std::string reason)
{
	CommandBase cmd;
	cmd.name = kHaltCommand;
//	cmd.payload = halt ? "true" : "false";
//	cmd.payload2 = runType;
//	cmd.context = reason;
	
//	CommandCenter::Instance()->BroadcastNow(cmd);
}

void Commands::VSync()
{
	CommandBase cmd;
	cmd.name = kVSyncCommand;
	
//	CommandCenter::Instance()->BroadcastNow(cmd);
}
