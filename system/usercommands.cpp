#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "usercommands.h"
#include "system/log.h"
#include "commandhelpers.h"

#define DEBUG 0

namespace fs = std::filesystem;

UserCommands *UserCommands::instance = nullptr;

UserCommands::UserCommands()
{
    {
        CommandInfo commandInfo;
        commandInfo.func = &UserCommands::Command_Quit;
        commandInfo.hint = "quit [return] - quits the application";
        commandHandlerMap[std::string("quit")] = commandInfo;
    }
    {
        CommandInfo commandInfo;
        commandInfo.func = &UserCommands::Command_Help;
        commandInfo.hint = "help <command> - prints help on specified command";
        commandHandlerMap[std::string("help")] = commandInfo;        
    }
    {
        CommandInfo commandInfo;
        commandInfo.func = &UserCommands::Command_Open;
        commandInfo.hint = "open <filename> - opens file";
        commandHandlerMap[std::string("open")] = commandInfo;
    }
    {
        CommandInfo commandInfo;
        commandInfo.func = &UserCommands::Command_Pwd;
        commandInfo.hint = "pwd - print current working directory";
        commandHandlerMap[std::string("pwd")] = commandInfo;
    }
    {
        CommandInfo commandInfo;
        commandInfo.func = &UserCommands::Command_Ls;
        commandInfo.hint = "ls - print contents of current working directory";
        commandHandlerMap[std::string("ls")] = commandInfo;
    }
}

UserCommands::~UserCommands()
{

}

void UserCommands::ParseAndProcessCommand(const std::string& command)
{
#if DEBUG
   	LOGINFOF("UserCommands::Processing command: '%s'", command.c_str());
#endif

    // split the string
    std::stringstream ss(command);
    std::string item;
    std::vector<std::string> commandComponents;
    while (std::getline(ss, item, ' '))
    {
        commandComponents.push_back(item);
    }

    // iterate through known handler methods and call id a match is found
    bool consumed = false;
    for (std::map<std::string, CommandInfo>::iterator iter = commandHandlerMap.begin(); iter != commandHandlerMap.end(); ++iter)
    {
        std::string command =  iter->first;
        if(commandComponents[0] == command)
        {
            UserCommands::CommandInfo handler = iter->second;
            (this->*handler.func)(commandComponents);
            consumed = true;
            break;
        }
    }
    // no match found so report error
    if(!consumed)
    {
        CommandHelpers::TextOutput("ERROR - Unknown command...");
    }
}

void UserCommands::OutputCompletions(const std::string& partialCommand)
{
    for (std::map<std::string, CommandInfo>::iterator iter = commandHandlerMap.begin(); iter != commandHandlerMap.end(); ++iter)
    {
        std::string command =  iter->first;
        if(command.find(partialCommand) == 0)
        {
            CommandHelpers::TextOutput(command);
        }
    }
}

void UserCommands::Command_Quit(const std::vector<std::string>& command)
{
    CommandHelpers::Quit();
}

void UserCommands::Command_Open(const std::vector<std::string>& command)
{
}

void UserCommands::Command_Ls(const std::vector<std::string>& command)
{
    std::string cwd = fs::current_path().string();
    for(const auto& entry : fs::directory_iterator(cwd))
    {
        CommandHelpers::TextOutput(entry.path().string());
    }
}

void UserCommands::Command_Pwd(const std::vector<std::string>& command)
{
    fs::path cwd = fs::current_path();
    CommandHelpers::TextOutput(cwd.string());
}

void UserCommands::Command_Help(const std::vector<std::string>& command)
{
    if(command.size() < 2)
    {
        CommandHelpers::TextOutput("Please specify a command you want help with... 'help [command]'");
    }
    else
    {
        std::string helpSubject = command[1];
        if(commandHandlerMap.find(helpSubject) != commandHandlerMap.end())
        {
            CommandHelpers::TextOutput(commandHandlerMap[helpSubject].hint);
        }
        else
        {
            // unknown command
            CommandHelpers::TextOutput("Unknown command");
        }
    }
}
