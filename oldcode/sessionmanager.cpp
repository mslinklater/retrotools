// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details
#if 0
#include "sessionmanager.h"
#include "system/commands.h"

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
//	CommandCenter::Instance()->Subscribe(kHaltCommand, this);
}

SessionManager::~SessionManager()
{

}

bool SessionManager::HandleCommand(const std::shared_ptr<CommandBase> command)
{
	return false;
}
#endif