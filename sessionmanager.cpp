#include "sessionmanager.h"
#include "commands.h"

SessionManager* SessionManager::pInstance = nullptr;

SessionManager* SessionManager::Instance()
{
	if(!pInstance)
	{
		pInstance = new SessionManager();
	}
	return pInstance;
}

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