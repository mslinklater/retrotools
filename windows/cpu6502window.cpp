#include <stdlib.h>
#include <stdio.h>

#include "cpu6502window.h"
#include "../imgui/imgui.h"

Cpu6502Window::Cpu6502Window()
{
}

Cpu6502Window::~Cpu6502Window()
{
}

void Cpu6502Window::SetCpu(Cpu6502* cpu)
{
	pCpu = cpu;
}

void Cpu6502Window::Draw()
{
	ImGui::Text("PC :");
	ImGui::SameLine();
	ImGui::Text("0x%04x", pCpu->GetPC());
	ImGui::SameLine();
	ImGui::Text("%d", pCpu->GetPC());

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
	ImGui::Text(status & Cpu6502::kNegativeSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502::kOverflowSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text("-");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502::kBreakSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502::kDecimalSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502::kInterruptSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502::kZeroSetMask ? "1" : "0");
	ImGui::SameLine();
	ImGui::Text(status & Cpu6502::kCarrySetMask ? "1" : "0");
	
	if(ImGui::Button("Next"))
	{
		pCpu->ProcessInstruction();
	}

	if(ImGui::Button("Next 100"))
	{
		for(int i=0 ; i<100 ; i++)
		{
			pCpu->ProcessInstruction();
		}
	}
}
