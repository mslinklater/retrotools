#pragma once

#include <string>

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

    void ParseAndProcessCommand(const std::string command);

private:
    static UserCommands *instance;
};
