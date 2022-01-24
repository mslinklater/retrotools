// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
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

// Text Output

class TextOutputCommand : public CommandBase
{
public:
	static const std::string kName;

	TextOutputCommand(std::string _text)
	: CommandBase(kName)
	, text(_text)
	{}

	virtual ~TextOutputCommand(){}
	std::string text;
};

// Scroll to bottom

class ScrollToBottomCommand : public CommandBase
{
public:
	static const std::string kName;

	ScrollToBottomCommand()
	: CommandBase(kName)
	{}

	virtual ~ScrollToBottomCommand(){}
};

// Quit

class QuitCommand : public CommandBase
{
public:
	static const std::string kName;

	QuitCommand()
	: CommandBase(kName)
	{}
};

// Open Resource Window

class OpenResourceWindowCommand : public CommandBase
{
public:
	static const std::string kName;

	OpenResourceWindowCommand(std::string _resourceId)
	: CommandBase(kName)
	, resourceId(_resourceId)
	{}

	std::string resourceId;
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

