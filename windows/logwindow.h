// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include "windowmanager.h"
#include "windowbase.h"

class LogWindow : public WindowBase, public IStateSerialisation
{
public:
	LogWindow();
	virtual ~LogWindow();
	
	// WindowBase
	virtual void Draw();
	// ~WindowBase

	ISERIALISATION_API

private:
	bool showInfo;
	bool showWarnings;
	bool showErrors;
};
