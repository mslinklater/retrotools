// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "resourcewindowbase.h"

//class ResourceT64;

class BinaryWindow : public ResourceWindowBase
{
public:
	BinaryWindow();
	virtual ~BinaryWindow();
	
	virtual void Draw(void);
	
private:
};
