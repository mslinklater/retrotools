// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "system/window/windowbase.h"
#include "resources/resourcemanager.h"

class ResourcesWindow : public WindowBase
{
public:
	ResourcesWindow();
	virtual ~ResourcesWindow();
	
	virtual void Draw(void);
private:
};
