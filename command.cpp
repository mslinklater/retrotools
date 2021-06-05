// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "command.h"
#include "common.h"
#include <algorithm>

using namespace std;

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
		shared_ptr<CommandBase> thisCommand = commandList[readQueueIndex].front();
		commandList[readQueueIndex].pop();
		
		if(dispatchMap.find(thisCommand->name) != dispatchMap.end())
		{
			vector<ICommandHandler*> handlers = dispatchMap[thisCommand->name];
			for( ICommandHandler* handler : handlers)
			{
				handler->HandleCommand(thisCommand);
			}
		}
	}
}

void CommandCenter::QueueForBroadcast(shared_ptr<CommandBase> command)
{
#if LOGGING
	LOGINFOF("CommandCenter::QueueForBroadcast %s(%s,%s)", command.name.c_str(), command.payload.c_str(), command.payload2.c_str());
#endif
	commandList[writeQueueIndex].push(command);
}

void CommandCenter::BroadcastNow(shared_ptr<CommandBase> thisCommand)
{
#if LOGGING
	LOGINFOF("CommandCenter::BroadcastNow %s(%s,%s)", thisCommand.name.c_str(), thisCommand.payload.c_str(), thisCommand.payload2.c_str());
#endif
	if(dispatchMap.find(thisCommand->name) != dispatchMap.end())
	{
		vector<ICommandHandler*> handlers = dispatchMap[thisCommand->name];
		for( ICommandHandler* handler : handlers)
		{
			handler->HandleCommand(thisCommand);
		}
	}
}

CommandCenter * CommandCenter::Instance()
{
	if(pInstance == nullptr)
	{
		pInstance = new CommandCenter();
	}
	return pInstance;
}

void CommandCenter::Subscribe(string commandName, ICommandHandler* handler)
{
	vector<ICommandHandler*>& vec = dispatchMap[commandName];

	if(find(vec.begin(), vec.end(), handler) == vec.end())
	{
		vec.push_back(handler);
	}
	else
	{
		LOGERRORF("Subscribing to the same command twice:%s", commandName.c_str());
	}
	
}

void CommandCenter::Unsubscribe(string commandName, ICommandHandler* handler)
{
	vector<ICommandHandler*>& vec = dispatchMap[commandName];

	auto ret = find(vec.begin(), vec.end(), handler);

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

void SharedCommands::ToggleWindow(string windowName)
{
#if LOGGING
	LOGINFOF("Commands::ToggleWindow %s", windowName.c_str());
#endif

	ToggleWindowCommand cmd;
	cmd.name = kToggleWindowCommand;
	cmd.windowName = windowName;
	CommandCenter::Instance()->QueueForBroadcast(dynamic_pointer_cast<CommandBase>(make_shared<ToggleWindowCommand>(cmd)));
}

void SharedCommands::Quit(void)
{
#if LOGGING
	LOGINFO("Commands::Quit");
#endif

	CommandBase cmd;
	cmd.name = kQuitCommand;
	CommandCenter::Instance()->QueueForBroadcast(make_shared<CommandBase>(cmd));
}
