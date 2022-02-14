// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "system/window/windowmanager.h"
#include "system/window/windowbase.h"

class LuaWindow : public WindowBase
{
public:
	
	LuaWindow();
	virtual ~LuaWindow();
	
	virtual void Draw(void);

    int CommandPromptCallback(ImGuiInputTextCallbackData* data);

private:
	static const int kInputBufferSize = 256;
	char inputBuffer[kInputBufferSize];

};
