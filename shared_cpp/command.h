// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <map>
#include <queue>
#include "icommandprocessor.h"

namespace SharedCommands
{
	void ToggleWindow(std::string windowName);
	static const std::string kToggleWindowCommand("ToggleWindow");

	void Quit(void);
	static const std::string kQuitCommand("Quit");
}

class CommandCenter
{
public:
	// Yes, singleton. Shoot me.
	static CommandCenter* Instance();
	void Update();
	
	void QueueForBroadcast(Command& command);
	void BroadcastNow(Command& command);
	
	void Subscribe(std::string commandName, ICommandProcessor* handler);
	void Unsubscribe(std::string commandName, ICommandProcessor* handler);
	
private:
	CommandCenter();
	virtual ~CommandCenter();
	
	uint32_t			writeQueueIndex;
	uint32_t			readQueueIndex;
	std::queue<Command> commandList[2];	// double-buffered to stop infinite recursion
	
	std::map<std::string,std::vector<ICommandProcessor*>>	dispatchMap;
	
	static CommandCenter* pInstance;
};
