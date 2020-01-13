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
, cpuTickDelay(0)
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
		running = (command.payload == "false");
	}
	return false;
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

	while((tickedUpToTime <= tickUpToTime) && running)
	{
		pTia->Tick();
		if(!pTia->IsCpuStalled())
		{
			if(cpuTickDelay == 0)
			{
				pCpu6502->Tick();
				cpuTickDelay = 3;
			}
			cpuTickDelay--;
		}
		
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
