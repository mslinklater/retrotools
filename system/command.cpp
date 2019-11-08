#include "command.h"

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
		
		printf("Processing command %s\n", thisCommand.name.c_str());
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
