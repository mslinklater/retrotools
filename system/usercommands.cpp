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
#include "system/common.h"
#include "message/messagehelpers.h"
#include "resources/resourcemanager.h"
#include "system/formatting.h"
#include "system/application.h"
#include "system/lua/luavm.h"

namespace fs = std::filesystem;

UserCommands *UserCommands::instance = nullptr;

static int lua_Quit( lua_State* pState )
{
	LUA_FUNCDEF("quit");
	LUA_ASSERT_NUMPARAMS(0);

    MessageHelpers::Quit();
    return 0;
}

static int lua_LoadLua( lua_State* pState )
{
	LUA_FUNCDEF("loadlua");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);
	std::string filename(lua_tostring(pState, -1));

	Application::Instance()->GetLua()->LoadScript(filename);
    return 0;
}

static int lua_ResOpen( lua_State* pState )
{
    return 0;
}

static int lua_ResClose( lua_State* pState )
{
    return 0;
}

static int lua_ResLoad( lua_State* pState )
{
    return 0;
}

UserCommands::UserCommands()
{
    Application::Instance()->GetLua()->RegisterCFunction(lua_LoadLua, "loadlua");
    Application::Instance()->GetLua()->RegisterCFunction(lua_Quit, "quit");

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

    {
        CommandInfo commandInfo;
        commandInfo.command = "resopen";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_ResOpen;
        commandInfo.completionFunctionPtr = &UserCommands::Completion_ResOpen;
        commandInfo.hint = "resopen <filename> [ID]- opens a resource file";
        commandInfo.helpText.push_back("[ID] - optionalal ID used to specify the resource.");
        commandInfo.helpText.push_back("       if not provided one will be generated.");
        AddToCommandHandlerMap(commandInfo);
        Application::Instance()->GetLua()->RegisterCFunction(lua_ResOpen, "resopen");
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "resclose";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_ResClose;
        commandInfo.completionFunctionPtr = &UserCommands::Completion_ResClose;
        commandInfo.hint = "resclose <ID>- closes a resource";
        AddToCommandHandlerMap(commandInfo);
        Application::Instance()->GetLua()->RegisterCFunction(lua_ResClose, "resclose");
    }
    {
        CommandInfo commandInfo;
        commandInfo.command = "resload";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_ResLoad;
        commandInfo.completionFunctionPtr = &UserCommands::Completion_ResLoad;
        commandInfo.hint = "resload [ID].[subID]- load a resource payload to memory";
        AddToCommandHandlerMap(commandInfo);
        Application::Instance()->GetLua()->RegisterCFunction(lua_ResLoad, "resload");
    }
    // File operations commands
    {
        CommandInfo commandInfo;
        commandInfo.command = "pwd";
        commandInfo.type = Type::EFileOperation;
        commandInfo.handlerFunctionPtr = &UserCommands::Command_Pwd;
        commandInfo.hint = "pwd - print current working directory";
        AddToCommandHandlerMap(commandInfo);
//        Application::Instance()->GetLua()->RegisterCFunction(lua_Pwd, "pwd");
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
//        Application::Instance()->GetLua()->RegisterCFunction(lua_Cd, "cd");
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
#if defined(DEBUG)
   	LOGINFOF("UserCommands::Processing command: '%s'", command.c_str());
#endif

    if(Application::Instance()->GetLua()->ExecuteLine(command) == kError_OK)
	{
		// command has been serviced by Lua so no need to continue to the C++ handlers
		return;
	}

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
        MessageHelpers::TextOutput("ERROR - Unknown command...");
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
            if(command == partialCommand)
            {
                // if user has the complete command, add the space
                completions.push_back(command + " ");
            }
            else if(command.find(partialCommand) == 0)
            {
                // if user has partial command fill in the whole command word
                completions.push_back(command);
            }
        }
        if(completions.size() == 1)
        {
            partialCommand = completions[0];
            completions.clear();
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

void UserCommands::Command_ResOpen(const std::vector<std::string>& command)
{
    if(command.size() > 2) // user provided an ID
    {
        ResourceManager::Instance()->OpenResourceFromFile(command[1], command[2]);
    }
    else
    {
        ResourceManager::Instance()->OpenResourceFromFile(command[1]);
    }
}

void UserCommands::Completion_ResOpen(std::string& parameters, std::vector<std::string>& completions)
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
        parameters = std::string("resopen ") + completions[0];
        completions.clear();
    }
    if(completions.size() > 1)
    {
        completions.push_back(" ");
    }

    return;
}

void UserCommands::Command_ResLoad(const std::vector<std::string>& command)
{

}

void UserCommands::Completion_ResLoad(std::string& parameters, std::vector<std::string>& completions)
{

}

void UserCommands::Command_ResClose(const std::vector<std::string>& command)
{
    LOGINFO("UserCommands::Command_ResClose");
    if(command.size() > 1)
    {
        const std::string resourceId = command[1];
        auto resourceMap = ResourceManager::Instance()->GetResources();
        auto res = resourceMap.find(resourceId);
        if(res != resourceMap.end())
        {
            // found the resource
			// close window
			MessageHelpers::CloseResourceWindow(resourceId);
			// close resource
			ResourceManager::Instance()->CloseResource(resourceId);
        }
        else
        {
            // cannot find requested resource
            MessageHelpers::TextOutput("Unknown resource ID");
        }
    }
}

void UserCommands::Completion_ResClose(std::string& parameters, std::vector<std::string>& completions)
{
    LOGINFO("UserCommands::Completion_ResClose");
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
            folders.push_back(TextFormat::kBoldTag + folderName.substr(cwdStripSize));
        }
        else
        {
            files.push_back(entry.path().string().substr(cwdStripSize));
        }
    }
    std::sort(folders.begin(), folders.end());
    for(auto& folder : folders)
    {
        MessageHelpers::TextOutput(folder);
    }
    std::sort(files.begin(), files.end());
    for(auto& file : files)
    {
        MessageHelpers::TextOutput(file);
    }
    MessageHelpers::ScrollToBottom();
}

void UserCommands::Command_Cd(const std::vector<std::string>& command)
{
    if(command.size() == 2)
    {
        fs::path newPath = fs::current_path() /= command[1];
        if(fs::exists(newPath))
        {
            fs::current_path(newPath);
            MessageHelpers::TextOutput(std::string("OK: ") + fs::current_path().string());
        }
        else
        {
            MessageHelpers::TextOutput(std::string("Unknown path: ") + newPath.string());
            MessageHelpers::TextOutput(fs::current_path().string());
        }
        MessageHelpers::ScrollToBottom();
    }
}

void UserCommands::Completion_Cd(std::string& parameters, std::vector<std::string>& completions)
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
        parameters = std::string("cd ") + completions[0];
        completions.clear();
    }
    if(completions.size() > 1)
    {
        completions.push_back(" ");
    }

    return;
}

void UserCommands::Command_Pwd(const std::vector<std::string>& command)
{
    MessageHelpers::TextOutput(fs::current_path().string());
    MessageHelpers::ScrollToBottom();
}

void UserCommands::Command_Help(const std::vector<std::string>& command)
{
    if(command.size() < 2)
    {
        MessageHelpers::TextOutput("Please specify a command you want help with... 'help [command]'");
    }
    else
    {
        std::string helpSubject = command[1];
        if(commandHandlerMap.find(helpSubject) != commandHandlerMap.end())
        {
            MessageHelpers::TextOutput(commandHandlerMap[helpSubject].hint);
        }
        else
        {
            // unknown command
            MessageHelpers::TextOutput("Unknown command");
        }
    }
    MessageHelpers::ScrollToBottom();
}

void UserCommands::Completion_Help(std::string& parameters, std::vector<std::string>& completions)
{
    return;
}
