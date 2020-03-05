// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "system.h"
#include "tia.h"
#include "cpu6502.h"
#include "riot.h"
#include "../shared_cpp/log.h"
#include "../commands.h"

System::System()
: running(false)
, tickFrequency(kCoreFrequencyNTSC)
, tickFrequencyMultiplier(1.0f)
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
	switch(pTia->GetRegion())
	{
		case Tia::ERegion::NTSC:
			tickFrequency = kCoreFrequencyNTSC * tickFrequencyMultiplier;
			break;
		case Tia::ERegion::PAL:
		case Tia::ERegion::SECAM:
			tickFrequency = kCoreFrequencyPAL * tickFrequencyMultiplier;
			break;
	}

	deltaTPerTick = 1.0 / tickFrequency;

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
//		if(!pTia->IsCpuStalled() || (pCpu6502->GetTicksUntilExecution() > 1))	// If CPU instruction is in-flight, let it complete before HSYNC stops it.
		if(!pTia->IsCpuStalled())	// If CPU instruction is in-flight, let it complete before HSYNC stops it.
		{
			if(cpuTickDelay == 0)
			{
				pCpu6502->Tick(false);
				pCpu6502->Tick(true);
				
				pRiot->Tick();
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

void System::Reset()
{
	pCpu6502->SetPC(0xf000);
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

void System::SetRiot(Riot* pRiotIn)
{
    pRiot = pRiotIn;
}
