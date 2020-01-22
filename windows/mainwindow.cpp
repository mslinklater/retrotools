// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "mainwindow.h"
#include "../shared_cpp/log.h"
#include "../imgui/imgui.h"
#include "../shared_cpp/command.h"
#include "../commands.h"
#include "../shared_cpp/windowmanager.h"
#include "../filebrowser/ImGuiFileBrowser.h"

imgui_addons::ImGuiFileBrowser file_dialog; // As a class member or globally

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
	
	bool open = false;

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
				open = true;
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
				SharedCommands::Quit();
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Export"))
		{
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Windows"))
		{
			for(std::string name : pWindowManager->GetWindows())
			{
				if(ImGui::MenuItem(name.c_str()))
				{
					SharedCommands::ToggleWindow(name);
				}	   
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}	

	if(open)
	{
		ImGui::OpenPopup("Open File");
	}

	if(file_dialog.showOpenFileDialog("Open File", ImVec2(700, 310), ".bin,.prg"))
	{
		LOGINFOF("MainWindow::Open file %s", file_dialog.selected_fn.c_str());
	}
	ImGui::End();
}
