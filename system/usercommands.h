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
    // Handler function ptr
    // In - const vector of strings
    // Out - void
    typedef void (UserCommands::*HandlerFunctionPtr)(const std::vector<std::string> &);

    // Completion function ptr
    // In - const string - the user input
    // In - (output)vector of strings ref to be filled with possible completions
    typedef void (UserCommands::*CompletionFunctionPtr)(const std::string&, std::vector<std::string>&);

    enum class Type
    {
        EGeneral,
        EFileOperation
    };

    struct CommandInfo
    {
        CommandInfo()
        : type(Type::EGeneral)
        , handlerFunctionPtr(nullptr)
        , completionFunctionPtr(nullptr)
        {}
        
        Type type;
        HandlerFunctionPtr handlerFunctionPtr;
        CompletionFunctionPtr completionFunctionPtr;
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
    void GetCompletions(std::string& command, std::vector<std::string>& completions);

    const std::vector<CommandInfo> GetCommandInfo();

private:
    static UserCommands *instance;

    std::map<std::string, CommandInfo> commandHandlerMap;
    void AddToCommandHandlerMap(const CommandInfo &info);

    void Command_Help(const std::vector<std::string>& command);
    void Completion_Help(const std::string& parameters, std::vector<std::string>& completions);

    void Command_History(const std::vector<std::string>& command);

    void Command_Open(const std::vector<std::string>& command);
    void Completion_Open(const std::string& parameters, std::vector<std::string>& completions);

    void Command_Quit(const std::vector<std::string>& command);
    void Command_Pwd(const std::vector<std::string>& command);
    void Command_Ls(const std::vector<std::string>& command);

    void Command_Cd(const std::vector<std::string>& command);
    void Completion_Cd(const std::string& parameters, std::vector<std::string>& completions);
};
