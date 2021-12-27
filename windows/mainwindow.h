// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include "common.h"
#include "system/icommandhandler.h"

class WindowManager;

class MainWindow : public ICommandHandler
{
public:
	MainWindow();
	virtual ~MainWindow();
	
	void Draw();
	void SetWindowManager(WindowManager* wm){ pWindowManager = wm; }

	ICOMMANDHANDLER_API

private:
	bool open;
	WindowManager* pWindowManager;
	bool bShowNewSession;
	bool bScrollToBottom;

	static const int kNewFilenameMaxLength = 256;
	char newFilename[kNewFilenameMaxLength];

	void ClearNewFilename()
	{
		for(int i=0 ; i<kNewFilenameMaxLength ; i++) newFilename[i] = 0;
	}

	void DrawMenuBar();

	// Console stuff
public:
    int CommandPromptCallback(ImGuiInputTextCallbackData* data);

private:
	static const int kInputBufferSize = 256;
	char inputBuffer[kInputBufferSize];

	std::vector<std::string> commandHistory;	// Command history
	int commandHistoryPtr;
	std::vector<std::string> outputItems;

	void DrawConsole();
	void ClearOutput();
};
