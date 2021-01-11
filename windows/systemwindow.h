// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "../shared_cpp/windowbase.h"
#include "../shared_cpp/command.h"

class System;

class SystemWindow : public WindowBase, public ICommandProcessor
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
	virtual bool HandleCommand(const std::shared_ptr<Command> command);
	// ~ICommandProcessor

	void DrawState();
	void DrawTIA();
	void DrawCPU();
	void DrawSpeedControl();
};
