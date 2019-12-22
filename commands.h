// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>

namespace Commands
{
	void NewProject(void);
	static const std::string kNewProjectCommand("NewProject");

	void ToggleWindow(std::string windowName);
	static const std::string kToggleWindowCommand("ToggleWindow");

	void Quit(void);
	static const std::string kQuitCommand("Quit");

	void Halt(bool halt);
	static const std::string kHaltCommand("Halt");

};
