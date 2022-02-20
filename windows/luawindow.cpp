// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "system/common.h"
#include "luawindow.h"
#include "system/lua/luavm.h"

LuaWindow::LuaWindow()
{
	memset(&inputBuffer[0], 0, kInputBufferSize);
}

LuaWindow::~LuaWindow()
{
}

static int CommandPromptCallbackStub(ImGuiInputTextCallbackData* data)
{
	LuaWindow* window = (LuaWindow*)data->UserData;
	return window->CommandPromptCallback(data);
}

int LuaWindow::CommandPromptCallback(ImGuiInputTextCallbackData* data)
{
#if 0
	if(data->EventFlag & ImGuiInputTextFlags_CallbackHistory)	// history
	{
		// only if we actually have a history to go through
		if(commandHistory.size() > 1)
		{
			if(data->EventKey == ImGuiKey_UpArrow)
			{
				commandHistoryPtr = std::max(commandHistoryPtr - 1, 0);
			}
			if(data->EventKey == ImGuiKey_DownArrow)
			{
				commandHistoryPtr = std::min(commandHistoryPtr + 1, (int)commandHistory.size()-1);			
			}
			strncpy(data->Buf, commandHistory[commandHistoryPtr].c_str(), data->BufSize);
			data->BufTextLen = commandHistory[commandHistoryPtr].length();
			data->BufDirty = true;
		}
	}
	if(data->EventFlag & ImGuiInputTextFlags_CallbackCompletion)	// completion
	{
		// find num matches - if one, call that completion handler... if many, display the options.
		std::vector<std::string> matches;
		std::string commandBuffer(data->Buf);
		UserCommands::Instance()->GetCompletions(commandBuffer, matches);

		// update the command prompt
		strncpy(data->Buf, commandBuffer.c_str(), data->BufSize);
		data->BufTextLen = commandBuffer.length();
		data->CursorPos = data->BufTextLen;
		data->BufDirty = true;

		// display the matches
		for(const auto &item : matches)
		{
			CommandHelpers::TextOutput(item);
		}
	}
#endif
	return 0;
}

void LuaWindow::Draw(void)
{
	ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;

	if (ImGui::InputText(" ", inputBuffer, kInputBufferSize, inputTextFlags, &CommandPromptCallbackStub, (void *)this))
	{
//		LOGINFOF("Lua::%s", &inputBuffer[0]);
		pLua->ExecuteLine(&inputBuffer[0]);
	}

	for(auto s : LuaVM::printOutputBuffer)
	{
		ImGui::Text("%s", s.c_str());
	}
}

