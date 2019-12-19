#pragma once

class ITickable
{
    public:

        enum Mode
        {
            ETickTIA,
            ETickCPU,
            ETickCPUInstruction,
            ETickHBlank,
            ETickVBlank,
            ETickRunning
        };

        virtual void Tick(Mode tickMode) = 0;
};
