// [CopyrightNotice]
// Copyright (c) Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "messagecenter.h"
#include "system/common.h"
#include <algorithm>

using namespace std;

#define LOGGING 0

MessageCenter* MessageCenter::pInstance = 0;

MessageCenter::MessageCenter()
	: writeQueueIndex(0)
	, readQueueIndex(1)
{
}

MessageCenter::~MessageCenter()
{
}

void MessageCenter::Update()
{
	// toggle queue indexes
	readQueueIndex = 1-readQueueIndex;
	writeQueueIndex = 1-readQueueIndex;
	// empty the read queue
	
	while(!messageList[readQueueIndex].empty())
	{
		shared_ptr<MessageBase> thisCommand = messageList[readQueueIndex].front();
		messageList[readQueueIndex].pop();
		
		if(dispatchMap.find(thisCommand->name) != dispatchMap.end())
		{
			vector<IMessageHandler*> handlers = dispatchMap[thisCommand->name];
			for( IMessageHandler* handler : handlers)
			{
				handler->HandleMessage(thisCommand);
			}
		}
	}
}

void MessageCenter::QueueForBroadcast(shared_ptr<MessageBase> command)
{
#if LOGGING
	LOGINFOF("CommandCenter::QueueForBroadcast %s(%s,%s)", command.name.c_str(), command.payload.c_str(), command.payload2.c_str());
#endif
	messageList[writeQueueIndex].push(command);
}

void MessageCenter::BroadcastNow(shared_ptr<MessageBase> thisCommand)
{
#if LOGGING
	LOGINFOF("CommandCenter::BroadcastNow %s(%s,%s)", thisCommand.name.c_str(), thisCommand.payload.c_str(), thisCommand.payload2.c_str());
#endif
	if(dispatchMap.find(thisCommand->name) != dispatchMap.end())
	{
		vector<IMessageHandler*> handlers = dispatchMap[thisCommand->name];
		for( IMessageHandler* handler : handlers)
		{
			handler->HandleMessage(thisCommand);
		}
	}
}

MessageCenter * MessageCenter::Instance()
{
	if(pInstance == nullptr)
	{
		pInstance = new MessageCenter();
	}
	return pInstance;
}

void MessageCenter::Subscribe(string commandName, IMessageHandler* handler)
{
	vector<IMessageHandler*>& vec = dispatchMap[commandName];

	if(find(vec.begin(), vec.end(), handler) == vec.end())
	{
		vec.push_back(handler);
	}
	else
	{
		LOGERRORF("CommandCenter::Subscribing to the same command twice:%s", commandName.c_str());
	}
	
}

void MessageCenter::Unsubscribe(string commandName, IMessageHandler* handler)
{
	vector<IMessageHandler*>& vec = dispatchMap[commandName];

	auto ret = find(vec.begin(), vec.end(), handler);

	if(ret != vec.end())
	{
		vec.erase(ret);
	}
	else
	{
		LOGERRORF("CommandCenter::Unsubscribing when not subscribed:%s", commandName.c_str());
	}
}
