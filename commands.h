// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include "icommandhandler.h"

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
	
	void VSync();
	static const std::string kVSyncCommand("VSync");
};
