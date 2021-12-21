// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "commandhelpers.h"

void CommandHelpers::ToggleWindow(std::string windowName)
{
    std::shared_ptr<CommandBase> cmd = std::make_shared<ToggleWindowCommand>(windowName);
	CommandCenter::Instance()->BroadcastNow(cmd);
}
