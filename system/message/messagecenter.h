// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <map>
#include <queue>
#include "system/message/imessagehandler.h"

/**
 * @brief The message center is the backbone of the subscriber pattern used in retrotools.
 * 
 * Message center deals with internal commands. User commands, entered via the console, are
 * handled by the UserCommands system.
 */

class MessageCenter
{
public:
	// Yes, singleton. Shoot me.
	static MessageCenter* Instance();
	void Update();
	
	void QueueForBroadcast(std::shared_ptr<MessageBase> message);
	void BroadcastNow(std::shared_ptr<MessageBase> message);
	
	void Subscribe(std::string messageName, IMessageHandler* handler);
	void Unsubscribe(std::string messageName, IMessageHandler* handler);

private:
	MessageCenter();
	virtual ~MessageCenter();
	
	uint32_t			writeQueueIndex;
	uint32_t			readQueueIndex;
	std::queue<std::shared_ptr<MessageBase>> messageList[2];	// double-buffered to stop infinite recursion
	
	std::map<std::string,std::vector<IMessageHandler*>>	dispatchMap;
	
	static MessageCenter* pInstance;

	// debug

	bool logging;
};
