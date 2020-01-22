#include "riot.h"

Riot::Riot()
{
	// clear memory
	for(int i=0 ; i<kMemorySize ; i++)
	{
		memory[i] = 0;
	}
	// defaults
	memory[kSWCHB - kMemorySize] = kSWCHB_SELECT_MASK | kSWCHB_SELECT_MASK;
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
	memory[addr - kMemoryStart] = val;
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
