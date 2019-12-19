// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "../system/windowbase.h"

class System;

class SystemWindow : public WindowBase
{
public:
	
	SystemWindow();
	virtual ~SystemWindow();

	void SetSystem(System* system);
	
	virtual void Draw(void);

private:
	System*		pSystem;
};
