// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>
#include "../interfaces/itickable.h"
#include "system/icommandhandler.h"

class Riot : public ITickable, public ICommandHandler
{
public:

	static const int kMemoryStart = 0x0280;
	static const int kMemorySize = 0x18;

	static const int kSWCHA = 0x280;	// Port A data register for joysticks
	static const int kSWACNT = 0x281;	// Port A data direction register
	static const int kSWCHB = 0x282;	// Port B data (console switches)
	static const int kSWBCNT = 0x283;	// Port B DDR
	static const int kINTIM = 0x284;	// Timer output
	static const int kTIMINT = 0x285;	// 
	static const int kTIM1T = 0x294;	// Set 1 clock interval
	static const int kTIM8T = 0x295;	// Set 8 clock interval
	static const int kTIM64T = 0x296;	// Set 64 clock interval
	static const int kT1024T = 0x297;	// Set 1024 clock interval

	// port b bit masks
	static const int kSWCHB_RESET_MASK = 0x01;
	static const int kSWCHB_SELECT_MASK = 0x02;
	static const int kSWCHB_COLORBW_MASK = 0x08;
	static const int kSWCHB_P0DIFF_MASK = 0x40;
	static const int kSWCHB_P1DIFF_MASK = 0x80;

	Riot();
	virtual ~Riot();

	uint8_t Read(uint16_t address);
	uint8_t DbgRead(uint16_t address);	// used by UI etc
	void	Write(uint16_t address, uint8_t value);
	void	DbgWrite(uint16_t address, uint8_t value);	// used by UI etc

	bool GetBreakpoint(uint16_t addr);
	void SetBreakpoint(uint16_t addr, bool val);

    // ITickable
	void CommitInputs(){}		// commit state of input pins - so chip update order doesn't matter
	void Tick(bool clockState){}	// update the actual silicon state - based on the clockState
    // ~ITickable

    void Tick();

	ICOMMANDHANDLER_API;

	uint8_t GetTimerVal(){return timerVal;}
	uint16_t GetTimerInterval(){return timerInterval;}
	uint16_t GetTimerDelay(){return timerDelay;}

private:
	uint8_t	memory[kMemorySize];
	bool	breakpoints[kMemorySize];

	uint8_t timerVal;
	uint16_t timerInterval;
	uint16_t timerDelay;
};
