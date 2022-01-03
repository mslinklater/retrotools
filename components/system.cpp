// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#include "common.h"
#include "system.h"
#include "stella/tia.h"
#include "cpu6502/cpu6502alpha.h"
#include "stella/riot.h"
#include "system/commands.h"

System::System()
: running(false)
, tickFrequency(kCoreFrequencyNTSC)
, tickFrequencyMultiplier(1.0f)
, tickedUpToTime(0.0)
, tickUpToTime(0.0)
, deltaTPerTick(1.0 / tickFrequency)
, cpuTickDelay(0)
{
//	CommandCenter::Instance()->Subscribe(Commands::kHaltCommand, this);
}

System::~System()
{

}

bool System::HandleCommand(const std::shared_ptr<CommandBase> command)
{
#if 0
	if(command.name == Commands::kHaltCommand)
	{
		running = (command.payload == "false");
	}
#endif
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
		if(!pTia->IsCpuStalled())	// If CPU instruction is in-flight, let it complete before HSYNC stops it.
		{
			if(cpuTickDelay == 0)
			{
				pCpu6502Alpha->Tick(false);
				pCpu6502Alpha->Tick(true);
				
				pRiot->Tick();
				cpuTickDelay = 3;
			}
			cpuTickDelay--;
		}
		
		tickedUpToTime += deltaTPerTick;
		numTicks++;
	}
}

void System::Init()
{
    pCpu6502Alpha->Init(Cpu6502Base::k6507);
	pCpu6502Alpha->SetMemory(pMemory);
	pCpu6502Alpha->SetPC(0xf000);
//	pMemory->SetCPU(pCpu6502);
//	pMemory->SetTia(pTia);
//	pMemory->SetRiot(pRiot);
}

void System::Tick()
{
}

void System::Reset()
{
	pCpu6502Alpha->SetPC(0xf000);
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

void System::SetCpu6502Alpha(Cpu6502Alpha* pCpu6502In)
{
    pCpu6502Alpha = pCpu6502In;
}

void System::SetRiot(Riot* pRiotIn)
{
    pRiot = pRiotIn;
}

void System::SetMemory(IMemory* pMemoryIn)
{
    pMemory = pMemoryIn;
}
