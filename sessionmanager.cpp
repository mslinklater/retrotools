#include "sessionmanager.h"
#include "commands.h"

SessionManager::SessionManager()
{
	CommandCenter::Instance()->Subscribe(Commands::kHaltCommand, this);
}

SessionManager::~SessionManager()
{

}

bool SessionManager::HandleCommand(const Command& command)
{
	return false;
}
