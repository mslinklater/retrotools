// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "system.h"
#include "tia.h"
#include "cpu6502.h"
#include "../shared_cpp/log.h"
#include "../commands.h"

System::System()
: running(false)
, tickFrequency(3584160)
, tickedUpToTime(0.0)
, tickUpToTime(0.0)
, deltaTPerTick(1.0 / tickFrequency)
{
	CommandCenter::Instance()->Subscribe(Commands::kHaltCommand, this);
}

System::~System()
{

}

bool System::HandleCommand(const Command& command)
{
	if(command.name == Commands::kHaltCommand)
	{
		running = (command.payload == "true");
	}
	return false;
}

#if 0
void System::TickTia()
{
	Commands::Halt(false, Commands::kHaltCommandTickTia);
	running = true;
}

void System::TickCpu()
{
	Commands::Halt(false, Commands::kHaltCommandTickCpu);
	running = true;
}

void System::TickCpuInstruction()
{
	Commands::Halt(false, Commands::kHaltCommandCpuInstruction);
	running = true;
}

void System::TickHBlank()
{
	Commands::Halt(false, Commands::kHaltCommandHBlank);
	running = true;
}

void System::TickVBlank()
{
	Commands::Halt(false, Commands::kHaltCommandVBlank);
	running = true;
}
#endif

void System::Run()
{
	Commands::Halt(false, Commands::kHaltCommandRun);
	running = true;
}

void System::Halt()
{
	running = false;
	Commands::Halt(true);
}

void System::Update(float dt)
{
	if(!running)
	{
		return;
	}

	updatedt = dt;
	tickUpToTime += (double)dt;

	uint32_t numTicks = 0;

	while(tickedUpToTime <= tickUpToTime)
	{
		pTia->Tick();
		tickedUpToTime += deltaTPerTick;
		numTicks++;
	}
}

void System::Tick()
{
//	pTia->Tick();
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
