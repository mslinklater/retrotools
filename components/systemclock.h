// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class Tia;
class Cpu6502;

class SystemClock
{
public:
    SystemClock();
    virtual ~SystemClock();

    void TickTia();
    void TickCpu();
    void AdvanceRaster();
    void AdvanceFrame();

    void SetTia(Tia* pTia);
    void SetCpu6502(Cpu6502* pCpu6502);

private:
    Tia*        pTia;
    Cpu6502*    pCpu6502;
};
