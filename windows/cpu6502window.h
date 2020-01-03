#pragma once

#include "../shared_cpp/windowbase.h"
#include "../components/cpu6502.h"

class Cpu6502Window : public WindowBase
{
public:
	Cpu6502Window();
	virtual ~Cpu6502Window();
	
	void Draw() override;
	
	void SetCpu(Cpu6502* cpu);
	
private:
	Cpu6502* pCpu;
	
	static const int kNewBreakpointTextSize = 5;
	char	newBreakpointText[kNewBreakpointTextSize];
};
