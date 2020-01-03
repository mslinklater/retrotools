// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "../shared_cpp/windowbase.h"

class Tia;

class TiaWindow : public WindowBase
{
public:
	TiaWindow();
	virtual ~TiaWindow();
	
	virtual void Draw(void);

    void SetTia(Tia* tia);

private:
    Tia* pTia;
};
