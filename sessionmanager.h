// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details
#if 0
#pragma once

#include "system/commandcenter.h"

class SessionManager : public ICommandHandler
{
public:
	static SessionManager* Instance();

private:
	SessionManager();
	virtual ~SessionManager();

	static SessionManager* pInstance;

	// ICommandProcessor
	bool HandleCommand(const std::shared_ptr<CommandBase> command) override;
	// ~ICommandProcessor
};
#endif