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
			pSystem->Halt();
		}
		ImGui::PopStyleColor(1);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if(ImGui::Button("HALTED!"))
		{
			pSystem->Run();
		}
		ImGui::PopStyleColor(1);
	}
    ImGui::Text("dt:%f", pSystem->GetUpdateDT());
    ImGui::Text("fps:%f", 1.0f / pSystem->GetUpdateDT());
    ImGui::Separator();
    ImGui::Text("TIA");
    if(ImGui::Button("Tick"))
    {
        pSystem->TickTia();
    }
    ImGui::SameLine();
    if(ImGui::Button("HBlank"))
    {
        pSystem->TickHBlank();
    }
    ImGui::SameLine();
    if(ImGui::Button("VBlank"))
    {
        pSystem->TickVBlank();
    }
    ImGui::Separator();
    ImGui::Text("CPU");
    if(ImGui::Button("Tick"))
    {
        pSystem->TickCpu();
    }
    ImGui::SameLine();
    if(ImGui::Button("Instruction"))
    {
        pSystem->TickCpuInstruction();
    }
}

