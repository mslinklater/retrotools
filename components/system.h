// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once

#include <vector>
#include <inttypes.h>
#include "system/icommandhandler.h"

class ITickable;
class IMemory;
class Tia;
class Cpu6502Alpha;
class Riot;

class System : public ICommandHandler
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
    void SetCpu6502Alpha(Cpu6502Alpha* pCpu6502);
    void SetRiot(Riot* pRiot);
    void SetMemory(IMemory* pMemory);

    void Update(float dt);
    float GetUpdateDT(void);
	bool GetRunning();

	void Tick();

	void SetTickFrequencyMultiplier(float multiplier){tickFrequencyMultiplier = multiplier;}
	uint32_t GetTickFrequency(){return tickFrequency;}

	// ICommandProcessor
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command);
	// ~ICommandProcessor

	void Reset();

private:
    Tia*        	pTia;
    Cpu6502Alpha*    pCpu6502Alpha;
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
