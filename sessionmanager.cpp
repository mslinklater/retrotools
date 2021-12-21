// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

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

bool SessionManager::HandleCommand(const std::shared_ptr<CommandBase> command)
{
	return false;
}
