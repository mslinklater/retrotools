// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "messagehelpers.h"

std::shared_ptr<MessageCenter> MessageHelpers::messageCenter;

void MessageHelpers::ToggleWindow(const std::string& windowName)
{
    std::shared_ptr<MessageBase> cmd = std::make_shared<ToggleWindowMessage>(windowName);
	messageCenter->BroadcastNow(cmd);
}

void MessageHelpers::TextOutput(const std::string& text)
{
    std::shared_ptr<MessageBase> cmd = std::make_shared<TextOutputMessage>(text);
	messageCenter->BroadcastNow(cmd);
}

void MessageHelpers::ScrollToBottom()
{
    std::shared_ptr<MessageBase> cmd = std::make_shared<ScrollToBottomMessage>();
	messageCenter->BroadcastNow(cmd);
}

void MessageHelpers::Quit()
{
    std::shared_ptr<MessageBase> cmd = std::make_shared<QuitMessage>();
	messageCenter->BroadcastNow(cmd);
}

void MessageHelpers::OpenResourceWindow(const std::string& resourceId)
{
    std::shared_ptr<MessageBase> cmd = std::make_shared<OpenResourceWindowMessage>(resourceId);
	messageCenter->BroadcastNow(cmd);
}

void MessageHelpers::CloseResourceWindow(const std::string& resourceId)
{
    std::shared_ptr<MessageBase> cmd = std::make_shared<CloseResourceWindowMessage>(resourceId);
	messageCenter->BroadcastNow(cmd);
}
