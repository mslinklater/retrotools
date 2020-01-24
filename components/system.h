// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <inttypes.h>
#include "../shared_cpp/icommandprocessor.h"

class ITickable;
class Tia;
class Cpu6502;

class System : public ICommandProcessor
{
public:

	static const int kCoreFrequency = 3584160;

    System();
    virtual ~System();

    void SetTia(Tia* pTia);
    void SetCpu6502(Cpu6502* pCpu6502);

    void Update(float dt);
    float GetUpdateDT(void);
	bool GetRunning();

	void Tick();

	void SetTickFrequency(uint32_t ticksPerSecond){tickFrequency = ticksPerSecond;}
	uint32_t GetTickFrequency(){return tickFrequency;}

	// ICommandProcessor
	virtual bool HandleCommand(const Command& command);
	// ~ICommandProcessor

	void Reset();

private:
    Tia*        pTia;
    Cpu6502*    pCpu6502;
    float		updatedt;
    bool		running;
	uint32_t	tickFrequency;
	double		tickedUpToTime;
	double		tickUpToTime;
	double		deltaTPerTick;
	uint32_t	cpuTickDelay;	
};
