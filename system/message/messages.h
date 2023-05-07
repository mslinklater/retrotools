// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include "system/message/imessagehandler.h"

// Toggle Window

class ToggleWindowMessage : public MessageBase
{
public:
	static const std::string kName;

	ToggleWindowMessage(std::string _windowName)
	: MessageBase(kName)
	, windowName(_windowName)
	{}

	virtual ~ToggleWindowMessage(){}
	std::string windowName;
};

// Text Output

class TextOutputMessage : public MessageBase
{
public:
	static const std::string kName;

	TextOutputMessage(std::string _text)
	: MessageBase(kName)
	, text(_text)
	{}

	virtual ~TextOutputMessage(){}
	std::string text;
};

// Scroll to bottom

class ScrollToBottomMessage : public MessageBase
{
public:
	static const std::string kName;

	ScrollToBottomMessage()
	: MessageBase(kName)
	{}

	virtual ~ScrollToBottomMessage(){}
};

// Quit

class QuitMessage : public MessageBase
{
public:
	static const std::string kName;

	QuitMessage()
	: MessageBase(kName)
	{}
};

// Open Resource Window

class OpenResourceWindowMessage : public MessageBase
{
public:
	static const std::string kName;

	OpenResourceWindowMessage(std::string _resourceId)
	: MessageBase(kName)
	, resourceId(_resourceId)
	{}

	std::string resourceId;
};

// Close resource window
class CloseResourceWindowMessage : public MessageBase
{
public:
	static const std::string kName;

	CloseResourceWindowMessage(std::string _resourceId)
	: MessageBase(kName)
	, resourceId(_resourceId)
	{}

	std::string resourceId;
};

// Halt

class HaltMessage : public MessageBase
{
public:
	static const std::string kName;

	HaltMessage()
	: MessageBase(kName)
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

