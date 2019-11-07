#pragma once

#include <string>
#include <map>

struct Command
{
	std::string	name;
	std::map<std::string, std::string> payload;
};

class CommandCenter
{
public:
	CommandCenter();
	virtual ~CommandCenter();
private:
	
};
