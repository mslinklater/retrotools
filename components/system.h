// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>

class ITickable;
class Tia;
class Cpu6502;

class System
{
public:

    System();
    virtual ~System();

    void TickTia();
    void TickCpu();
    void TickCpuInstruction();
    void TickHBlank();
    void TickVBlank();
    void Run();

    void SetTia(Tia* pTia);
    void SetCpu6502(Cpu6502* pCpu6502);

    void Update(float dt);
    float GetUpdateDT(void);
	bool GetRunning();

private:
    Tia*        pTia;
    Cpu6502*    pCpu6502;
    float		updatedt;
    bool		running;
};
