#pragma once
#include <string>
#include <memory>

class CommandBase
{
public:
	virtual ~CommandBase(){}
	std::string	name;
};

class ICommandProcessor
{
public:
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command) = 0;
};
