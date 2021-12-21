// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "windowmanager.h"
#include "windowbase.h"

class HelpAboutWindow : public WindowBase, public IStateSerialisation
{
public:
	
	HelpAboutWindow();
	virtual ~HelpAboutWindow();
	
	virtual void Draw(void);

	ISERIALISATION_API

private:
	
};
