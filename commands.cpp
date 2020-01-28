// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commands.h"
#include "shared_cpp/command.h"
#include "shared_cpp/log.h"

void Commands::Halt(bool halt, std::string runType, std::string reason)
{
	Command cmd;
	cmd.name = kHaltCommand;
	cmd.payload = halt ? "true" : "false";
	cmd.payload2 = runType;

	if(reason == "unknown")
	{
		int bp=0;
		bp++;
	}

	LOGINFOF("Halt::Due to %s", reason.c_str());

	CommandCenter::Instance()->BroadcastNow(cmd);
}
