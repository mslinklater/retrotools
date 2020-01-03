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


	void Halt(bool halt, std::string runType = "");
	static const std::string kHaltCommand("Halt");
	static const std::string kHaltCommandTickTia("TickTia");
	static const std::string kHaltCommandHBlank("HBlank");
	static const std::string kHaltCommandVBlank("VBlank");
	static const std::string kHaltCommandTickCpu("TickCpu");
	static const std::string kHaltCommandCpuInstruction("CpuInstruction");
	static const std::string kHaltCommandRun("Run");

};
