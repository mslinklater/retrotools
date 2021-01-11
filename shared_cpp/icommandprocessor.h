#pragma once
#include <string>

class Command
{
public:
	std::string	name;	
	std::string payload;
	std::string payload2;
	std::string context;
};

class ICommandProcessor
{
public:
	virtual bool HandleCommand(const Command& command) = 0;
};
