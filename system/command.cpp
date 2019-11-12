#include "command.h"
#include "../log.h"

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
			for( ICommandProcessor* handler : handlers)
			{
				handler->HandleCommand(thisCommand);
			}
		}
	}
}

void CommandCenter::Broadcast(Command& command)
{
	commandList[writeQueueIndex].push(command);
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
	dispatchMap[commandName].push_back(handler);
}
