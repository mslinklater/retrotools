// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "mainwindow.h"
#include "../imgui/imgui.h"
#include "../system/command.h"

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
				// new session
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
		if(ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}		
	
	
	if(ImGui::Button("Log"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Log";
		CommandCenter::Instance()->Broadcast(msg);
	}
	if(ImGui::Button("Memory"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Memory";
		CommandCenter::Instance()->Broadcast(msg);
	}
	if(ImGui::Button("Disassembly"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Disassembly";
		CommandCenter::Instance()->Broadcast(msg);
	}
	if(ImGui::Button("Symbols"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Symbols";
		CommandCenter::Instance()->Broadcast(msg);
	}
	ImGui::End();
}
