// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <inttypes.h>
#include "../shared_cpp/icommandprocessor.h"

class ITickable;
class IMemory;
class Tia;
class Cpu6502;
class Riot;

class System : public ICommandProcessor
{
public:

	static const int kCoreFrequencyNTSC = 262 * 228 * 60;
	static const int kCoreFrequencyPAL = 312 * 228 * 50;

    System();
    virtual ~System();

	void Init();	// initialise all the cross-component connections

	uint8_t	CpuRead(uint16_t address);
	void CpuWrite(uint16_t address, uint8_t value);

    void SetTia(Tia* pTia);
    void SetCpu6502(Cpu6502* pCpu6502);
    void SetRiot(Riot* pRiot);
    void SetMemory(IMemory* pMemory);

    void Update(float dt);
    float GetUpdateDT(void);
	bool GetRunning();

	void Tick();

	void SetTickFrequencyMultiplier(float multiplier){tickFrequencyMultiplier = multiplier;}
	uint32_t GetTickFrequency(){return tickFrequency;}

	// ICommandProcessor
	virtual bool HandleCommand(const Command& command);
	// ~ICommandProcessor

	void Reset();

private:
    Tia*        pTia;
    Cpu6502*    pCpu6502;
	Riot*		pRiot;
	IMemory*	pMemory;

    float		updatedt;
    bool		running;
	uint32_t	tickFrequency;
	float		tickFrequencyMultiplier;
	double		tickedUpToTime;
	double		tickUpToTime;
	double		deltaTPerTick;
	uint32_t	cpuTickDelay;	
};
