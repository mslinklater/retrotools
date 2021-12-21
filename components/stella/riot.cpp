// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "riot.h"
#include "../commands.h"

Riot::Riot()
: timerVal(1)
, timerInterval(1)
, timerDelay(1)
{
	// clear memory
	for(int i=0 ; i<kMemorySize ; i++)
	{
		memory[i] = 0;
		breakpoints[i] = false;
	}
	// defaults
	memory[kSWCHB - kMemoryStart] = kSWCHB_SELECT_MASK | kSWCHB_RESET_MASK | kSWCHB_COLORBW_MASK ;
}

Riot::~Riot()
{

}

uint8_t Riot::Read(uint16_t addr)
{
	if(breakpoints[addr])
	{
//		Commands::Halt(true, HaltCommand::kHalt, "RIOT read breakpoint");		
	}

	uint8_t ret = 0;

	switch(addr | 0x0280)
	{
		case kSWCHB:
			ret = 0x08 | 0x02 | 0x01;	// temp, no reset or select, colour output
			break;
		case kINTIM:
			ret = timerVal;
			break;
		default:
			break;
	}

	// affect flags etc
	return ret;
}

uint8_t Riot::DbgRead(uint16_t addr)
{
	uint8_t ret = 0;

	switch(addr | 0x0280)
	{
		case kINTIM:
			ret = timerVal;
			break;
		default:
			break;
	}

	// affect flags etc
	return ret;
}

void Riot::Write(uint16_t addr, uint8_t val)
{
	// affect flags etc
	switch(addr | 0x0280)
	{
		case kTIM1T:
			timerDelay = 1;
			timerInterval = 1;
			break;
		case kTIM8T:
			timerDelay = 8;
			timerInterval = 8;
			break;
		case kTIM64T:
			timerInterval = 64;
			timerDelay = 64;
			break;
		case kT1024T:
			timerDelay = 1024;
			timerInterval = 1024;
			break;
		default:	// no write
			break;
	}
	timerVal = val;

	if(breakpoints[addr])
	{
		//Commands::Halt(true, HaltCommand::kHalt, "RIOT write breakpoint");		
	}
}

void Riot::DbgWrite(uint16_t addr, uint8_t val)
{
	memory[addr & 0x003f] = val;
}

void Riot::Tick()
{
	// timer

	timerDelay--;
	if(timerDelay == 0)
	{
		timerDelay = timerInterval;
		timerVal--;
		if(timerVal == 0xff)	// reached the end of countdown so interval is now 1
		{
			timerInterval = 1;
			timerDelay = 1;
		}
	}


	// read input, deal with timers etc

}

bool Riot::HandleCommand(const std::shared_ptr<CommandBase> command)
{
	return false;
}

bool Riot::GetBreakpoint(uint16_t addr)
{
	return breakpoints[addr & 0x003f];
}

void Riot::SetBreakpoint(uint16_t addr, bool val)
{
	breakpoints[addr & 0x003f] = val;
}
