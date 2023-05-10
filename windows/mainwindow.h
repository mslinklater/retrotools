// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include "system/common.h"
#include "system/message/imessagehandler.h"

class WindowManager;
class MessageCenter;

struct MainWindowInit
{
	std::shared_ptr<MessageCenter> messageCenter;
};

class MainWindow : public IMessageHandler
{
public:
	MainWindow();
	virtual ~MainWindow();
	void Init(const MainWindowInit& init);
	
	void Draw();
	void SetWindowManager(std::shared_ptr<WindowManager> wm){ pWindowManager = wm; }

	IMESSAGEHANDLER_API

private:
	bool open;
	std::shared_ptr<WindowManager> pWindowManager;
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
