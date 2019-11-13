// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class LogWindow
{
public:
	LogWindow();
	virtual ~LogWindow();
	
	void Draw(bool* pOpen);
private:
	bool showInfo;
	bool showWarnings;
	bool showErrors;
};
