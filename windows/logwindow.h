// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "../system/windowmanager.h"
#include "../system/windowbase.h"

class LogWindow : public WindowBase
{
public:
	LogWindow();
	virtual ~LogWindow();
	
//	void Draw(bool* pOpen);
	
	// WindowBase
	virtual void Draw();
	// ~WindowBase
private:
	bool showInfo;
	bool showWarnings;
	bool showErrors;
};
