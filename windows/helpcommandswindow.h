// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "windowmanager.h"
#include "windowbase.h"

class HelpCommandsWindow : public WindowBase
{
public:
	
	HelpCommandsWindow();
	virtual ~HelpCommandsWindow();
	
	virtual void Draw(void);
		
private:
	
};
