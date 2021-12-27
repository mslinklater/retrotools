#pragma once

#include <string>
#include <vector>
#include <map>

class UserCommands
{
public:
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
    void OutputCompletions(const std::string& command);

private:
    static UserCommands *instance;

    typedef void (UserCommands::*HandlerFunction)(const std::vector<std::string> &);

    struct CommandInfo
    {
        HandlerFunction func;
        std::string hint;
    };

    std::map<std::string, CommandInfo> commandHandlerMap;

    void Command_Help(const std::vector<std::string>& command);
    void Command_Open(const std::vector<std::string>& command);
    void Command_Quit(const std::vector<std::string>& command);
    void Command_Pwd(const std::vector<std::string>& command);
    void Command_Ls(const std::vector<std::string>& command);
};
