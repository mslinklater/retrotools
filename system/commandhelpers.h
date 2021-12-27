// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "commandcenter.h"
#include "commands.h"

namespace CommandHelpers
{
    void ToggleWindow(std::string windowName);
    void TextOutput(std::string text);
    void ScrollToBottom();
    void Quit();
};
