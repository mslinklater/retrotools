// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "system.h"
#include "tia.h"
#include "cpu6502.h"
#include "../log.h"
#include "../commands.h"

System::System()
{

}

System::~System()
{

}

void System::TickTia()
{
	Commands::Halt(false, Commands::kHaltCommandTickTia);
}

void System::TickCpu()
{
	Commands::Halt(false, Commands::kHaltCommandTickCpu);
}

void System::TickCpuInstruction()
{
	Commands::Halt(false, Commands::kHaltCommandCpuInstruction);
}

void System::TickHBlank()
{
	Commands::Halt(false, Commands::kHaltCommandHBlank);
}

void System::TickVBlank()
{
	Commands::Halt(false, Commands::kHaltCommandVBlank);
}

void System::Run()
{
	Commands::Halt(false, Commands::kHaltCommandRun);
}

void System::Update(float dt)
{
	updatedt = dt;
}

float System::GetUpdateDT(void)
{
	return updatedt;
}

bool System::GetRunning()
{
	return running;
}

void System::SetTia(Tia* pTiaIn)
{
    pTia = pTiaIn;
}

void System::SetCpu6502(Cpu6502* pCpu6502In)
{
    pCpu6502 = pCpu6502In;
}
