#include "riot.h"

Riot::Riot()
{
	// clear memory
	for(int i=0 ; i<kMemorySize ; i++)
	{
		memory[i] = 0;
	}
	// defaults
	memory[kSWCHB - kMemoryStart] = kSWCHB_SELECT_MASK | kSWCHB_RESET_MASK;
}

Riot::~Riot()
{

}

uint8_t Riot::Read(uint16_t addr)
{
	// affect flags etc
	return memory[addr - kMemoryStart];
}

uint8_t Riot::DbgRead(uint16_t addr)
{
	return memory[addr - kMemoryStart];
}

void Riot::Write(uint16_t addr, uint8_t val)
{
	// affect flags etc
	switch(addr)
	{
		case kTIM1T:
		case kTIM8T:
		case kTIM64T:
		case kT1024T:
			memory[addr - kMemoryStart] = val;
			break;
		default:	// no write
			break;
	}
}

void Riot::DbgWrite(uint16_t addr, uint8_t val)
{
	memory[addr - kMemoryStart] = val;
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
	return false;
}

void Riot::SetBreakpoint(uint16_t addr, bool val)
{
	
}
