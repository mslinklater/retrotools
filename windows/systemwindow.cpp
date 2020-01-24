// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "systemwindow.h"
#include "../components/system.h"
#include "../imgui/imgui.h"
#include "../commands.h"

SystemWindow::SystemWindow()
{

}

SystemWindow::~SystemWindow()
{

}

void SystemWindow::SetSystem(System* system)
{
    pSystem = system;
}

void SystemWindow::DrawState()
{
	if(pSystem->GetRunning())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		if(ImGui::Button("RUNNING"))
		{
			Commands::Halt(true);
		}
		ImGui::PopStyleColor(1);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if(ImGui::Button("HALTED!"))
		{
			Commands::Halt(false, Commands::kHaltCommandRun);
		}
		ImGui::PopStyleColor(1);
	}
    ImGui::Text("dt:%f", pSystem->GetUpdateDT());
    ImGui::Text("fps:%f", 1.0f / pSystem->GetUpdateDT());
    ImGui::Separator();
}

void SystemWindow::DrawTIA()
{
	if(ImGui::TreeNode("TIA"))
	{
		if(ImGui::Button("TickTIA"))
		{
			Commands::Halt(false, Commands::kHaltCommandTickTia);
		}
		ImGui::SameLine();
		if(ImGui::Button("HBlank"))
		{
			Commands::Halt(false, Commands::kHaltCommandHBlank);
		}
		ImGui::SameLine();
		if(ImGui::Button("VBlank"))
		{
			Commands::Halt(false, Commands::kHaltCommandVBlank);
		}
        ImGui::TreePop();
	}
}

void SystemWindow::DrawCPU()
{
	if(ImGui::TreeNode("CPU"))
	{
		if(ImGui::Button("TickCPU"))
		{
			Commands::Halt(false, Commands::kHaltCommandTickCpu);
		}
		ImGui::SameLine();
		if(ImGui::Button("Instruction"))
		{
			Commands::Halt(false, Commands::kHaltCommandCpuInstruction);
		}
		ImGui::TreePop();
	}
}

void SystemWindow::DrawSpeedControl()
{
	static char newFreqText[16];
	if(ImGui::TreeNode("Speed"))
	{
		ImGui::Separator();
		ImGui::Text("Current Freq:%d", pSystem->GetTickFrequency());
		ImGui::PushItemWidth(40);
		ImGui::InputText("Hz", newFreqText, 7);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Set"))
		{
			uint32_t freq = strtol(newFreqText, NULL, 10);
			pSystem->SetTickFrequency(freq);
		}
		if(ImGui::Button("x1"))
		{
			pSystem->SetTickFrequency(System::kCoreFrequency);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/10"))
		{
			pSystem->SetTickFrequency(System::kCoreFrequency/10);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/100"))
		{
			pSystem->SetTickFrequency(System::kCoreFrequency/100);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/1K"))
		{
			pSystem->SetTickFrequency(System::kCoreFrequency/1000);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/10K"))
		{
			pSystem->SetTickFrequency(System::kCoreFrequency/10000);
		}

		ImGui::Separator();
		ImGui::TreePop();
	}
}

void SystemWindow::Draw(void)
{
	DrawState();
	DrawTIA();
	DrawCPU();
	DrawSpeedControl();
	if(ImGui::Button("Reset"))
	{
		pSystem->Reset();
	}
}

