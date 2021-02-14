#pragma once

#include "command.h"

class SessionManager : public ICommandProcessor
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
