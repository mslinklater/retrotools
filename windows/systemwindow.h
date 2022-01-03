// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "windowbase.h"
#include "system/commandcenter.h"

class System;

class SystemWindow : public WindowBase, public ICommandHandler
{
public:
	
	SystemWindow();
	virtual ~SystemWindow();

	void SetSystem(System* system);
	
	virtual void Draw(void);

private:
	System*		pSystem;
	std::string haltReason;

	// ICommandProcessor
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command);
	// ~ICommandProcessor

	void DrawState();
	void DrawTIA();
	void DrawCPU();
	void DrawSpeedControl();
};
