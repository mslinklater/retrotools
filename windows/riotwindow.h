// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "windowbase.h"
#include "istateserialisation.h"
#include "../components/stella/riot.h"

class RiotWindow : public WindowBase, public IStateSerialisation
{
public:
	RiotWindow();
	virtual ~RiotWindow();
	
	virtual void Draw(void);

    void SetRiot(Riot* riot);

	ISERIALISATION_API

private:
    Riot* pRiot;

	void DrawRegister(uint32_t reg, const char* text, uint8_t val);
};
