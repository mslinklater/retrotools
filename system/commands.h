// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include "icommandhandler.h"

// Toggle Window

class ToggleWindowCommand : public CommandBase
{
public:
	static const std::string kName;

	ToggleWindowCommand(std::string _windowName)
	: CommandBase(kName)
	, windowName(_windowName)
	{}

	virtual ~ToggleWindowCommand(){}
	std::string windowName;
};

// Halt

class HaltCommand : public CommandBase
{
public:
	static const std::string kName;

	HaltCommand()
	: CommandBase(kName)
	{}

	enum ERunType
	{
		kHalt,
		kTickTia,
		kHBlank,
		kVBlank,
		kTickCpu,
		kCpuInstruction,
		kRun
	};

	bool		halt;
	ERunType	runType;
	std::string reason;
};

