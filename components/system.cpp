// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "system.h"
#include "tia.h"
#include "cpu6502.h"
#include "../log.h"

System::System()
{

}

System::~System()
{

}

void System::TickTia()
{
    LOGINFO("System::TickTia");
}

void System::TickCpu()
{
    LOGINFO("System::TickCpu");
}

void System::TickCpuInstruction()
{
    LOGINFO("System::TickCpuInstruction");
}

void System::TickHBlank()
{
	LOGINFO("System::TickHBlank");
}

void System::TickVBlank()
{
	LOGINFO("System::TickVBlank");
}

void System::Update(float dt)
{
	updatedt = dt;
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

void System::SetCpu6502(Cpu6502* pCpu6502In)
{
    pCpu6502 = pCpu6502In;
}
