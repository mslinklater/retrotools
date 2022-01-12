// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

#include "usercommands.h"
#include "system/log.h"
#include "commandhelpers.h"
#include "resources/resourcemanager.h"

#define DEBUG 0

namespace fs = std::filesystem;

UserCommands *UserCommands::instance = nullptr;

UserCommands::UserCommands()
{
    // General commands
    {
        CommandInfo commandInfo;
        commandInfo.command = "quit";
        commandInfo.type = Type::EGeneral;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Quit;
        commandInfo.hint = "quit [return] - quits the application";
        AddToCommandHandlerMap(commandInfo);
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "help";
        commandInfo.type = Type::EGeneral;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Help;
        commandInfo.completionFunctionPtr = &UserCommands::Completion_Help;
        commandInfo.hint = "help <command> - prints help on specified command";
        AddToCommandHandlerMap(commandInfo);
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "history";
        commandInfo.type = Type::EGeneral;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_History;
        commandInfo.hint = "history [num] - display command history";
        commandInfo.helpText.push_back("[num] - optionally specify max number of entries to show");
        AddToCommandHandlerMap(commandInfo);
    }

    // File operations commands
    {
        CommandInfo commandInfo;
        commandInfo.command = "open";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Open;
        commandInfo.completionFunctionPtr = &UserCommands::Completion_Open;
        commandInfo.hint = "open <filename> - opens file";
        AddToCommandHandlerMap(commandInfo);
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "pwd";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Pwd;
        commandInfo.hint = "pwd - print current working directory";
        AddToCommandHandlerMap(commandInfo);
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "ls";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Ls;
        commandInfo.hint = "ls - print contents of current working directory";
        AddToCommandHandlerMap(commandInfo);
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "cd";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Cd;
        commandInfo.completionFunctionPtr = &UserCommands::Completion_Cd;
        commandInfo.hint = "cd - change current directory";
        AddToCommandHandlerMap(commandInfo);
    }
}

UserCommands::~UserCommands()
{

}

void UserCommands::AddToCommandHandlerMap(const CommandInfo &info)
{
    commandHandlerMap[info.command] = info;
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
            UserCommands::CommandInfo commandInfo = iter->second;
            (this->*commandInfo.handlerFunctionPtr)(commandComponents);
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

void UserCommands::GetCompletions(std::string& partialCommand, std::vector<std::string>& completions)
{
    // Need to strip anythig after a space, if there is one...

    std::size_t spacePos = partialCommand.find(' ');
    if(spacePos != std::string::npos)
    {
        // user wants help with command parameters
        std::string baseCommand = partialCommand.substr(0, spacePos);

        // check the base command is valid
        for (const auto &iter : commandHandlerMap)
        {
            std::string command =  iter.first;
            if(command.find(baseCommand) == 0)
            {
                // if valid, ask the base command completion handler for valid completions
                UserCommands::CommandInfo commandInfo = iter.second;
                if(commandInfo.completionFunctionPtr != nullptr)
                {
                    (this->*commandInfo.completionFunctionPtr)(partialCommand, completions);
                }
            }
        }

    }
    else
    {
        for (const auto &iter : commandHandlerMap)
        {
            std::string command =  iter.first;
            if(command.find(partialCommand) == 0)
            {
                completions.push_back(command);
            }
        }
    }

    return;
}

const std::vector<UserCommands::CommandInfo> UserCommands::GetCommandInfo()
{   
    std::vector<CommandInfo> ret;
    for(auto iter : commandHandlerMap)
    {
        ret.push_back(iter.second);
    }
    return ret;
}

void UserCommands::Command_Quit(const std::vector<std::string>& command)
{
    CommandHelpers::Quit();
}

void UserCommands::Command_Open(const std::vector<std::string>& command)
{
    ResourceManager::Instance()->OpenResourceFromFile(command[1]);
}

void UserCommands::Completion_Open(const std::string& parameters, std::vector<std::string>& completions)
{
    // grab the partial path and find the directory entries which pattern patch
    std::size_t spacePos = parameters.find(' ');
    std::string partialPath = parameters.substr(spacePos + 1);

    std::string cwd = fs::current_path().string();
    std::size_t cwdSize = cwd.size();
    for(const auto& entry : fs::directory_iterator(cwd))
    {
        if(fs::is_regular_file(entry.status()))
        {
            std::string comp = entry.path().string().substr(cwdSize+1);
            if(comp.find(partialPath) == 0)
            {
                completions.push_back(comp);
            }
        }
    }
    if(completions.size() == 1)
    {
        completions[0] = std::string("open ") + completions[0];
    }
    if(completions.size() > 1)
    {
        completions.push_back(" ");
    }

    return;
}

void UserCommands::Command_History(const std::vector<std::string>& command)
{
    LOGERROR("UserCommands::Issue#65 - Command_History");
}

void UserCommands::Command_Ls(const std::vector<std::string>& command)
{
    std::string cwd = fs::current_path().string();
    std::size_t cwdStripSize = cwd.size() + 1;
    std::vector<std::string> folders;
    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(cwd))
    {
        if(fs::is_directory(entry.status()))
        {
            std::string folderName = entry.path().string() + "/";
            // strip the leading path to cwd and store
            folders.push_back(folderName.substr(cwdStripSize));
        }
        else
        {
            files.push_back(entry.path().string().substr(cwdStripSize));
        }
    }
    std::sort(folders.begin(), folders.end());
    for(auto& folder : folders)
    {
        CommandHelpers::TextOutput(folder);
    }
    std::sort(files.begin(), files.end());
    for(auto& file : files)
    {
        CommandHelpers::TextOutput(file);
    }
    CommandHelpers::ScrollToBottom();
}

void UserCommands::Command_Cd(const std::vector<std::string>& command)
{
    if(command.size() == 2)
    {
        fs::path newPath = fs::current_path() /= command[1];
        if(fs::exists(newPath))
        {
            fs::current_path(newPath);
            CommandHelpers::TextOutput(std::string("OK: ") + fs::current_path().string());
        }
        else
        {
            CommandHelpers::TextOutput(std::string("Unknown path: ") + newPath.string());
            CommandHelpers::TextOutput(fs::current_path().string());
        }
        CommandHelpers::ScrollToBottom();
    }
}

void UserCommands::Completion_Cd(const std::string& parameters, std::vector<std::string>& completions)
{
    // grab the partial path and find the directory entries which pattern patch
    std::size_t spacePos = parameters.find(' ');
    std::string partialPath = parameters.substr(spacePos + 1);

    std::string cwd = fs::current_path().string();
    std::size_t cwdSize = cwd.size();
    for(const auto& entry : fs::directory_iterator(cwd))
    {
        if(fs::is_directory(entry.status()))
        {
            std::string comp = entry.path().string().substr(cwdSize+1);
            if(comp.find(partialPath) == 0)
            {
                completions.push_back(comp + "/");
            }
        }
    }
    if(completions.size() == 1)
    {
        completions[0] = std::string("cd ") + completions[0];
    }
    if(completions.size() > 1)
    {
        completions.push_back(" ");
    }

    return;
}

void UserCommands::Command_Pwd(const std::vector<std::string>& command)
{
    CommandHelpers::TextOutput(fs::current_path().string());
    CommandHelpers::ScrollToBottom();
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
    CommandHelpers::ScrollToBottom();
}

void UserCommands::Completion_Help(const std::string& parameters, std::vector<std::string>& completions)
{
    return;
}
