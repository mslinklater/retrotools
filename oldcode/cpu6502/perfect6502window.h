// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "system/window/windowbase.h"
#include "system/istateserialisation.h"
#include "perfect6502/perfect6502.h"

class Perfect6502Window : public WindowBase, public IStateSerialisation
{
public:
	Perfect6502Window();
	virtual ~Perfect6502Window();

	virtual void Draw() override;

	ISERIALISATION_API

private:

	bool bRunning;

	state_t* pState;
};