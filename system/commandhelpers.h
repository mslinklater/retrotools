// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "commandcenter.h"
#include "commands.h"

namespace CommandHelpers
{
    void ToggleWindow(const std::string& windowName);
    void TextOutput(const std::string& text);
    void ScrollToBottom();
    void Quit();
    void OpenResourceWindow(const std::string& resourceId);
    void CloseResourceWindow(const std::string& resourceId);
};
