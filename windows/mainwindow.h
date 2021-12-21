// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include "common.h"

class WindowManager;

class MainWindow
{
public:
	MainWindow();
	virtual ~MainWindow();
	
	void Draw();
	void SetWindowManager(WindowManager* wm){ pWindowManager = wm; }
private:
	bool open;
	WindowManager* pWindowManager;
	bool bShowNewSession;

	static const int kNewFilenameMaxLength = 256;
	char newFilename[kNewFilenameMaxLength];

	void ClearNewFilename()
	{
		for(int i=0 ; i<kNewFilenameMaxLength ; i++) newFilename[i] = 0;
	}

	void DrawMenuBar();

	// Console stuff
public:
    int TextEditCallback(ImGuiInputTextCallbackData* data);

private:
	static const int kInputBufferSize = 256;
	char inputBuffer[kInputBufferSize];

	ImVector<char*> history;	// Command history
	ImVector<char*> logItems;

 	void DrawConsole();
	void ClearLog();
};
