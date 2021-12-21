// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "common.h"
#include "commands.h"
#include "command.h"

void Commands::Halt(bool halt, Commands::HaltCommand::ERunType runType, std::string reason)
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
