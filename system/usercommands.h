// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <vector>
#include <map>

class UserCommands
{
public:
    typedef void (UserCommands::*HandlerFunction)(const std::vector<std::string> &);
    typedef void (UserCommands::*CompletionFunction)();

    enum class Type
    {
        EGeneral,
        EFileOperation
    };

    struct CommandInfo
    {
        CommandInfo()
        : type(Type::EGeneral)
        , func(nullptr)
        , completion(nullptr)
        {}
        
        Type type;
        HandlerFunction func;
        CompletionFunction completion;
        std::string command;
        std::string hint;
        std::vector<std::string> helpText;
    };

    UserCommands();
    virtual ~UserCommands();

    static UserCommands* Instance()
    {
        if(instance == nullptr)
        {
            instance = new UserCommands();
        }
        return instance;
    }

    void ParseAndProcessCommand(const std::string& command);
    const std::vector<std::string> GetCompletions(const std::string& command);

    const std::vector<CommandInfo> GetCommandInfo();

private:
    static UserCommands *instance;

    std::map<std::string, CommandInfo> commandHandlerMap;
    void AddToCommandHandlerMap(const CommandInfo &info);

    void Command_Help(const std::vector<std::string>& command);
    void Completion_Help();
    void Command_History(const std::vector<std::string>& command);

    void Command_Open(const std::vector<std::string>& command);
    void Command_Quit(const std::vector<std::string>& command);
    void Command_Pwd(const std::vector<std::string>& command);
    void Command_Ls(const std::vector<std::string>& command);
    void Command_Cd(const std::vector<std::string>& command);
};
