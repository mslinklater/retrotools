// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "command.h"
#include "log.h"
#include <algorithm>

#define LOGGING 0

CommandCenter* CommandCenter::pInstance = 0;

CommandCenter::CommandCenter()
	: writeQueueIndex(0)
	, readQueueIndex(1)
{
}

CommandCenter::~CommandCenter()
{
}

void CommandCenter::Update()
{
	// toggle queue indexes
	readQueueIndex = 1-readQueueIndex;
	writeQueueIndex = 1-readQueueIndex;
	// empty the read queue
	
	while(!commandList[readQueueIndex].empty())
	{
		Command thisCommand = commandList[readQueueIndex].front();
		commandList[readQueueIndex].pop();
		
		if(dispatchMap.find(thisCommand.name) != dispatchMap.end())
		{
			std::vector<ICommandProcessor*> handlers = dispatchMap[thisCommand.name];
//			for( ICommandProcessor* handler : handlers)
//			{
//				handler->HandleCommand(thisCommand);
//			}
		}
	}
}

void CommandCenter::QueueForBroadcast(std::shared_ptr<Command> command)
{
#if 0
#if LOGGING
	LOGINFOF("CommandCenter::QueueForBroadcast %s(%s,%s)", command.name.c_str(), command.payload.c_str(), command.payload2.c_str());
#endif
	commandList[writeQueueIndex].push(command);
#endif
}

void CommandCenter::BroadcastNow(std::shared_ptr<Command> thisCommand)
{
#if 0
#if LOGGING
	LOGINFOF("CommandCenter::BroadcastNow %s(%s,%s)", thisCommand.name.c_str(), thisCommand.payload.c_str(), thisCommand.payload2.c_str());
#endif
	if(dispatchMap.find(thisCommand.name) != dispatchMap.end())
	{
		std::vector<ICommandProcessor*> handlers = dispatchMap[thisCommand.name];
//		for( ICommandProcessor* handler : handlers)
//		{
//			handler->HandleCommand(thisCommand);
//		}
	}
#endif
}

CommandCenter * CommandCenter::Instance()
{
	if(pInstance == nullptr)
	{
		pInstance = new CommandCenter();
	}
	return pInstance;
}

void CommandCenter::Subscribe(std::string commandName, ICommandProcessor* handler)
{
	std::vector<ICommandProcessor*>& vec = dispatchMap[commandName];

	if(std::find(vec.begin(), vec.end(), handler) == vec.end())
	{
		vec.push_back(handler);
	}
	else
	{
		LOGERRORF("Subscribing to the same command twice:%s", commandName.c_str());
	}
	
}

void CommandCenter::Unsubscribe(std::string commandName, ICommandProcessor* handler)
{
	std::vector<ICommandProcessor*>& vec = dispatchMap[commandName];

	auto ret = std::find(vec.begin(), vec.end(), handler);

	if(ret != vec.end())
	{
		vec.erase(ret);
	}
	else
	{
		LOGERRORF("Unsubscribing when not subscribed:%s", commandName.c_str());
	}
}

// Shared Commands

void SharedCommands::ToggleWindow(std::string windowName)
{
#if LOGGING
	LOGINFOF("Commands::ToggleWindow %s", windowName.c_str());
#endif
	Command cmd;
	cmd.name = kToggleWindowCommand;
//	cmd.payload = windowName;
	CommandCenter::Instance()->QueueForBroadcast(std::make_shared<Command>(cmd));
}

void SharedCommands::Quit(void)
{
#if LOGGING
	LOGINFO("Commands::Quit");
#endif
	Command cmd;
	cmd.name = kQuitCommand;
	CommandCenter::Instance()->QueueForBroadcast(std::make_shared<Command>(cmd));
}
