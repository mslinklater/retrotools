// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <map>
#include <queue>

struct Command
{
	std::string	name;
	std::string payload;
	std::string payload2;
};

class ICommandProcessor
{
public:
	virtual bool HandleCommand(const Command& command) = 0;
};

class CommandCenter
{
public:
	// Yes, singleton. Shoot me.
	static CommandCenter* Instance();
	void Update();
	
	void Broadcast(Command& command);
	
	void Subscribe(std::string commandName, ICommandProcessor* handler);
	
private:
	CommandCenter();
	virtual ~CommandCenter();
	
	uint32_t			writeQueueIndex;
	uint32_t			readQueueIndex;
	std::queue<Command> commandList[2];	// double-buffered to stop infinite recursion
	
	std::map<std::string,std::vector<ICommandProcessor*>>	dispatchMap;
	
	static CommandCenter* pInstance;
};
