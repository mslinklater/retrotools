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
	CommandCenter::Instance()->Subscribe(Commands::kHaltCommand, this);
}

SystemWindow::~SystemWindow()
{

}

bool SystemWindow::HandleCommand(const std::shared_ptr<Command> command)
{
#if 0
	if(command.name == Commands::kHaltCommand)
	{
		haltReason = command.context;
	}
#endif
	return false;
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
			Commands::Halt(true, "", "System window button");
		}
		ImGui::PopStyleColor(1);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if(ImGui::Button("HALTED!"))
		{
			Commands::Halt(false, Commands::kHaltCommandRun, "System window button");
		}
		ImGui::SameLine();
		ImGui::Text("%s", haltReason.c_str());
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
			Commands::Halt(false, Commands::kHaltCommandTickTia, "System window TickTIA button");
		}
		ImGui::SameLine();
		if(ImGui::Button("HBlank"))
		{
			Commands::Halt(false, Commands::kHaltCommandHBlank, "System window HBlank button");
		}
		ImGui::SameLine();
		if(ImGui::Button("VBlank"))
		{
			Commands::Halt(false, Commands::kHaltCommandVBlank, "System window VBlank button");
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
			Commands::Halt(false, Commands::kHaltCommandTickCpu, "System window TickCPU button");
		}
		ImGui::SameLine();
		if(ImGui::Button("Instruction"))
		{
			Commands::Halt(false, Commands::kHaltCommandCpuInstruction, "System window CPU Instruction button");
		}
		ImGui::TreePop();
	}
}

void SystemWindow::DrawSpeedControl()
{
	if(ImGui::TreeNode("Speed"))
	{
		ImGui::Separator();
		if(ImGui::Button("x1"))
		{
			pSystem->SetTickFrequencyMultiplier(1.0f);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/10"))
		{
			pSystem->SetTickFrequencyMultiplier(1.0f / 10.0f);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/100"))
		{
			pSystem->SetTickFrequencyMultiplier(1.0f / 100.0f);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/1K"))
		{
			pSystem->SetTickFrequencyMultiplier(1.0f / 1000.0f);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/10K"))
		{
			pSystem->SetTickFrequencyMultiplier(1.0f / 10000.0f);
		}
		ImGui::SameLine();
		if(ImGui::Button("1/100K"))
		{
			pSystem->SetTickFrequencyMultiplier(1.0f / 100000.0f);
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

