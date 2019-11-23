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
};
