// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "system/window/windowmanager.h"
#include "system/window/windowbase.h"

class HelpAboutWindow : public WindowBase, public IStateSerialisation
{
public:
	
	HelpAboutWindow();
	virtual ~HelpAboutWindow();
	
	virtual void Draw(void);

	ISERIALISATION_API

private:
	
};
