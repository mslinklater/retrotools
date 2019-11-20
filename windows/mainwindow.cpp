// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "mainwindow.h"
#include "../imgui/imgui.h"
#include "../system/command.h"
#include "../commands.h"

MainWindow::MainWindow()
:open(true)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::Draw()
{
	ImGui::Begin("Vistella", &open, ImGuiWindowFlags_MenuBar);
	
	// Main Menu Bar
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Session"))
		{
			if(ImGui::MenuItem("New"))
			{
				Commands::NewProject();
			}
			if(ImGui::MenuItem("Open..."))
			{
				// open session
			}
			if(ImGui::MenuItem("Save"))
			{
				// save session
			}
			if(ImGui::MenuItem("Save & Quit"))
			{
				// save & quit session
			}
			if(ImGui::MenuItem("Quit"))
			{
				// quit session
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Export"))
		{
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Windows"))
		{
			if(ImGui::MenuItem("Log"))
			{
				Commands::ToggleWindow("Log");
			}
			if(ImGui::MenuItem("Memory"))
			{
				Commands::ToggleWindow("Memory");
			}
			if(ImGui::MenuItem("Disassembly"))
			{
				Commands::ToggleWindow("Disassembly");
			}
			if(ImGui::MenuItem("Symbols"))
			{
				Commands::ToggleWindow("Symbols");
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}		
	ImGui::End();
}
