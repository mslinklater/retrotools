#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "usercommands.h"
#include "system/log.h"

UserCommands *UserCommands::instance = nullptr;

UserCommands::UserCommands()
{

}

UserCommands::~UserCommands()
{

}

void UserCommands::ParseAndProcessCommand(const std::string command)
{
   	LOGINFOF("UserCommands::Processing command: '%s'", command.c_str());

    // split the string
    std::stringstream ss(command);
    std::string item;
    std::vector<std::string> commandComponents;
    while (std::getline(ss, item, ' '))
    {
        commandComponents.push_back(item);
    }
}
