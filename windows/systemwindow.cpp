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

void SystemWindow::Draw(void)
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
    ImGui::Text("TIA");
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
    ImGui::Separator();
    ImGui::Text("CPU");
    if(ImGui::Button("TickCPU"))
    {
		Commands::Halt(false, Commands::kHaltCommandTickCpu);
    }
    ImGui::SameLine();
    if(ImGui::Button("Instruction"))
    {
		Commands::Halt(false, Commands::kHaltCommandCpuInstruction);
    }
}

