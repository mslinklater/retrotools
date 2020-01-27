// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

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
};
