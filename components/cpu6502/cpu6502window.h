#pragma once

#include "windows/windowbase.h"
#include "components/cpu6502/cpu6502base.h"

class Cpu6502Window : public WindowBase
{
public:
	Cpu6502Window();
	virtual ~Cpu6502Window();
	
	void Draw() override;
	
	void SetCpu(Cpu6502Base* cpu);
	
private:
	Cpu6502Base* pCpu;
	
	static const int kNewBreakpointTextSize = 5;
	char	newBreakpointText[kNewBreakpointTextSize];
};
