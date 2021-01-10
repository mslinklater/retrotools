// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>

class WindowBase
{
public:
	WindowBase();
	virtual ~WindowBase();
	
	virtual void Draw(void) = 0;
private:	
};
