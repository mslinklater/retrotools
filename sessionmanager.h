#pragma once

#include "shared_cpp/command.h"

class SessionManager : public ICommandProcessor
{
public:
	SessionManager();
	virtual ~SessionManager();

private:
		// ICommandProcessor
		bool HandleCommand(const Command &command) override;
		// ~ICommandProcessor
};
