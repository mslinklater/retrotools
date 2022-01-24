// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commandhelpers.h"

void CommandHelpers::ToggleWindow(const std::string& windowName)
{
    std::shared_ptr<CommandBase> cmd = std::make_shared<ToggleWindowCommand>(windowName);
	CommandCenter::Instance()->BroadcastNow(cmd);
}

void CommandHelpers::TextOutput(const std::string& text)
{
    std::shared_ptr<CommandBase> cmd = std::make_shared<TextOutputCommand>(text);
	CommandCenter::Instance()->BroadcastNow(cmd);
}

void CommandHelpers::ScrollToBottom()
{
    std::shared_ptr<CommandBase> cmd = std::make_shared<ScrollToBottomCommand>();
	CommandCenter::Instance()->BroadcastNow(cmd);
}

void CommandHelpers::Quit()
{
    std::shared_ptr<CommandBase> cmd = std::make_shared<QuitCommand>();
	CommandCenter::Instance()->BroadcastNow(cmd);
}

void CommandHelpers::OpenResourceWindow(const std::string& _resourceId)
{
    std::shared_ptr<CommandBase> cmd = std::make_shared<OpenResourceWindowCommand>(_resourceId);
	CommandCenter::Instance()->BroadcastNow(cmd);
}

