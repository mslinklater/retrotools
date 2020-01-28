#include "riot.h"
#include "../commands.h"

Riot::Riot()
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
		Commands::Halt(true, "", "RIOT read breakpoint");		
	}

	// affect flags etc
	return memory[addr];
}

uint8_t Riot::DbgRead(uint16_t addr)
{
	return memory[addr & 0x003f];
}

void Riot::Write(uint16_t addr, uint8_t val)
{
	// affect flags etc
	switch(addr | 0x0280)
	{
		case kTIM1T:
		case kTIM8T:
		case kTIM64T:
		case kT1024T:
			memory[addr] = val;
			break;
		default:	// no write
			break;
	}
	if(breakpoints[addr])
	{
		Commands::Halt(true, "", "RIOT write breakpoint");		
	}
}

void Riot::DbgWrite(uint16_t addr, uint8_t val)
{
	memory[addr & 0x003f] = val;
}

void Riot::Tick()
{
	// read input, deal with timers etc
}

bool Riot::HandleCommand(const Command& command)
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
