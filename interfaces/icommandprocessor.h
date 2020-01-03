#pragma once
#include <string>

struct Command
{
	std::string	name;
	std::string payload;
	std::string payload2;
};

class ICommandProcessor
{
public:
	virtual bool HandleCommand(const Command& command) = 0;
};
