// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "../system/windowbase.h"

class SystemClock;

class SystemClockWindow : public WindowBase
{
public:
	
	SystemClockWindow();
	virtual ~SystemClockWindow();

	void SetSystemClock(SystemClock* clock);
	
	virtual void Draw(void);

private:
	SystemClock*		pClock;
};
