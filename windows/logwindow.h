// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "../shared_cpp/windowmanager.h"
#include "../shared_cpp/windowbase.h"

class LogWindow : public WindowBase, public IStateSerialisation
{
public:
	LogWindow();
	virtual ~LogWindow();
	
	// WindowBase
	virtual void Draw();
	// ~WindowBase

	// IStateSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IStateSerialisation

private:
	bool showInfo;
	bool showWarnings;
	bool showErrors;
};
