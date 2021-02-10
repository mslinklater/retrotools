// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include "shared_cpp/icommandprocessor.h"

namespace Commands
{
	class HaltCommand : public CommandBase
	{
	public:
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

	void Halt(bool halt, HaltCommand::ERunType runType, std::string reason);
	
	static const std::string kHaltCommand("Halt");
	
//	static const std::string kHaltCommandTickTia("TickTia");
//	static const std::string kHaltCommandHBlank("HBlank");
//	static const std::string kHaltCommandVBlank("VBlank");
//	static const std::string kHaltCommandTickCpu("TickCpu");
//	static const std::string kHaltCommandCpuInstruction("CpuInstruction");
//	static const std::string kHaltCommandRun("Run");

	void VSync();
	static const std::string kVSyncCommand("VSync");
};
