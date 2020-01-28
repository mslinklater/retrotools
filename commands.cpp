// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commands.h"
#include "shared_cpp/command.h"
#include "shared_cpp/log.h"

#if 0
void Commands::NewSession(std::string filename)
{
	LOGINFO("Commands::NewSession %s", filename.c_str());
	Command cmd;
	cmd.name = kNewSessionCommand;
	cmd.payload = filename;
	CommandCenter::Instance()->QueueForBroadcast(cmd);
}
#endif

void Commands::Halt(bool halt, std::string runType)
{
	Command cmd;
	cmd.name = kHaltCommand;
	cmd.payload = halt ? "true" : "false";
	cmd.payload2 = runType;

	CommandCenter::Instance()->BroadcastNow(cmd);
}
