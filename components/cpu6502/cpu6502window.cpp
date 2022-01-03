// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>
#include "cpu6502window.h"
#include "common.h"
#include "system/commands.h"

Cpu6502Window::Cpu6502Window()
{
	for(int i=0 ; i<kNewBreakpointTextSize ; i++)
	{
		newBreakpointText[i] = 0;
	}
}

Cpu6502Window::~Cpu6502Window()
{
}

void Cpu6502Window::SetCpu(Cpu6502Base* cpu)
{
	pCpu = cpu;
}

void Cpu6502Window::Draw()
{
	ImGui::Text("Cycles: %" PRId64, pCpu->GetCyclesSinceBoot());
	ImGui::Separator();
	ImGui::Text("PC :0x%04x", pCpu->GetPC());
//	ImGui::Text("Next :%d", pCpu->GetTicksUntilExecution());
	
	ImGui::Text("Acc:");
	ImGui::SameLine();
	ImGui::Text("0x%02x", pCpu->GetAcc());
	ImGui::SameLine();
	ImGui::Text("%d", pCpu->GetAcc());

	ImGui::Text("X  :");
	ImGui::SameLine();
	ImGui::Text("0x%02x", pCpu->GetX());
	ImGui::SameLine();
	ImGui::Text("%d", pCpu->GetX());

	ImGui::Text("Y  :");
	ImGui::SameLine();
	ImGui::Text("0x%02x", pCpu->GetY());
	ImGui::SameLine();
	ImGui::Text("%d", pCpu->GetY());

	ImGui::Text("SP :");
	ImGui::SameLine();
	ImGui::Text("0x%02x", pCpu->GetSP());
	ImGui::SameLine();
	ImGui::Text("%d", pCpu->GetSP());

	ImGui::Text("N");
	ImGui::SameLine();
	ImGui::Text("V");
	ImGui::SameLine();
	ImGui::Text("-");
	ImGui::SameLine();
	ImGui::Text("B");
	ImGui::SameLine();
	ImGui::Text("D");
	ImGui::SameLine();
	ImGui::Text("I");
	ImGui::SameLine();
	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::Text("C");
	
	uint8_t status = pCpu->GetStatus();
	ImGui::Text(status & Cpu6502Base::kNegativeSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502Base::kOverflowSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text("-");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502Base::kBreakSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502Base::kDecimalSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502Base::kInterruptSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502Base::kZeroSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502Base::kCarrySetMask ? "1" : "0");
	
	ImGui::Separator();
	ImGui::PushItemWidth(40);
	ImGui::InputText("Address", newBreakpointText, 5);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if(ImGui::Button("Add Breakpoint"))
	{
		// grab value and add a breakpoint
		uint16_t addr = (int)strtol(newBreakpointText, NULL, 16);
		pCpu->SetBreakpoint(addr);

		for(int i=0 ; i<kNewBreakpointTextSize ; i++)
		{
			newBreakpointText[i] = 0;
		}
	}

	ImGui::Separator();
	ImGui::Text("BREAKPOINTS");
	std::set<uint16_t> breakpoints = pCpu->GetBreakpoints();
	for(auto bp : breakpoints)
	{
		ImGui::Text("0x%04x", bp);
		ImGui::SameLine();
		if(ImGui::Button("Delete"))
		{
			pCpu->ClearBreakpoint(bp);
		}
	}
}
