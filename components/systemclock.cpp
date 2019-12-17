// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "systemclock.h"
#include "tia.h"
#include "cpu6502.h"

SystemClock::SystemClock()
{

}

SystemClock::~SystemClock()
{

}

void SystemClock::TickTia()
{

}

void SystemClock::TickCpu()
{

}

void SystemClock::AdvanceRaster()
{

}

void SystemClock::AdvanceFrame()
{

}

void SystemClock::SetTia(Tia* pTiaIn)
{
    pTia = pTiaIn;
}

void SystemClock::SetCpu6502(Cpu6502* pCpu6502In)
{
    pCpu6502 = pCpu6502In;
}
