// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <map>
#include <queue>
#include "icommandhandler.h"


class CommandCenter
{
public:
	// Yes, singleton. Shoot me.
	static CommandCenter* Instance();
	void Update();
	
	void QueueForBroadcast(std::shared_ptr<CommandBase> command);
	void BroadcastNow(std::shared_ptr<CommandBase> command);
	
	void Subscribe(std::string commandName, ICommandHandler* handler);
	void Unsubscribe(std::string commandName, ICommandHandler* handler);
	
private:
	CommandCenter();
	virtual ~CommandCenter();
	
	uint32_t			writeQueueIndex;
	uint32_t			readQueueIndex;
	std::queue<std::shared_ptr<CommandBase>> commandList[2];	// double-buffered to stop infinite recursion
	
	std::map<std::string,std::vector<ICommandHandler*>>	dispatchMap;
	
	static CommandCenter* pInstance;
};
