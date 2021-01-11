#pragma once
#include <string>
#include <memory>

class Command
{
public:
	std::string	name;
};

class ICommandProcessor
{
public:
	virtual bool HandleCommand(const std::shared_ptr<Command> command) = 0;
};
