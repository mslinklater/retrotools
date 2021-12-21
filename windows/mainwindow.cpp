// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <algorithm>

#include "mainwindow.h"
#include "common.h"
#include "system/commandcenter.h"
#include "system/commands.h"
#include "windowmanager.h"
#include "filebrowser/ImGuiFileBrowser.h"
#include "system/commandhelpers.h"
#include "system/usercommands.h"

//imgui_addons::ImGuiFileBrowser file_dialog; // As a class member or globally

MainWindow::MainWindow()
: open(true)
, bShowNewSession(false)
, commandHistoryPtr(0)
{
	memset(&inputBuffer[0], 0, kInputBufferSize);
}

MainWindow::~MainWindow()
{
}

void MainWindow::DrawMenuBar()
{
	// Main Menu Bar
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Session"))
		{
			if(ImGui::MenuItem("New..."))
			{
				bShowNewSession = true;
				ClearNewFilename();
			}
			if(ImGui::MenuItem("Open..."))
			{
				// open session
//				bSessionOpen = true;
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
//				SharedCommands::Quit();
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
					CommandHelpers::ToggleWindow(name);
				}
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Help"))
		{
			if(ImGui::MenuItem("Commands"))
			{
				CommandHelpers::ToggleWindow("HelpCommands");
			}
			if(ImGui::MenuItem("About"))
			{
				CommandHelpers::ToggleWindow("HelpAbout");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}	
}

static int CommandPromptCallbackStub(ImGuiInputTextCallbackData* data)
{
	MainWindow* window = (MainWindow*)data->UserData;
	return window->CommandPromptCallback(data);
}

int MainWindow::CommandPromptCallback(ImGuiInputTextCallbackData* data)
{
	if(data->EventFlag & ImGuiInputTextFlags_CallbackHistory)	// history
	{
		if(data->EventKey == ImGuiKey_UpArrow)
		{
			commandHistoryPtr = std::max(commandHistoryPtr - 1, 0);
		}
		if(data->EventKey == ImGuiKey_DownArrow)
		{
			commandHistoryPtr = std::min(commandHistoryPtr + 1, (int)commandHistory.size()-1);			
		}
		strncpy(data->Buf, commandHistory[commandHistoryPtr].c_str(), data->BufSize);
		data->BufTextLen = commandHistory[commandHistoryPtr].length();
		data->BufDirty = true;
	}
	return 0;
}

void MainWindow::ClearLog()
{

}

void MainWindow::DrawConsole()
{
	ImGui::Separator();
	// Scroll region

    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, 0);
	// the actual log
	for(std::size_t i=0; i<logItems.size(); i++)
	{
		const char* item = logItems[i].c_str();
		// TODO: Need to add some formatting here
		ImGui::TextUnformatted(item);
	}
	ImGui::EndChild();

	ImGui::Separator();
	// Command-Line
	bool reclaimFocus = false;
	ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;

	if(ImGui::InputText("command", inputBuffer, kInputBufferSize, inputTextFlags, &CommandPromptCallbackStub, (void*)this))
	{
		// add to command history
		std::string historyString(&inputBuffer[0]);
		commandHistory.push_back(historyString);

		// add to logItems
		char* newBuffer = new char[strlen(inputBuffer) + 3];
		sprintf(newBuffer, "> %s", inputBuffer);
		std::string newString(&newBuffer[0]);
		logItems.push_back(newString);
		delete newBuffer;

		UserCommands::Instance()->ParseAndProcessCommand(std::string(inputBuffer));
//		CommandCenter::Instance()->ParseAndProcessCommand(std::string(inputBuffer));

		// clear the buffer
		strcpy(inputBuffer, "");
		reclaimFocus = true;

		commandHistoryPtr = commandHistory.size();
	}

	ImGui::SetItemDefaultFocus();
    if (reclaimFocus)
	{
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	}
}

void MainWindow::Draw()
{
	ImGui::Begin("Retrotools", &open, ImGuiWindowFlags_MenuBar);
	
	bool bSessionOpen = false;

	DrawMenuBar();

	DrawConsole();

	// New Session

	if(bShowNewSession)
	{
		ImGui::Separator();
		ImGui::Text("New Session");
		ImGui::Separator();

		ImGui::PushItemWidth(100);
		ImGui::Text("Filename:");
		ImGui::SameLine();
		ImGui::InputText("", newFilename, kNewFilenameMaxLength);
		ImGui::PopItemWidth();

		if(ImGui::Button("Cancel"))
		{
			bShowNewSession = false;
		}
		ImGui::SameLine();
		if(ImGui::Button("Create"))
		{
			bShowNewSession = false;

//			Commands::NewProject(newFilename);
		}

		ImGui::Separator();
	}

	// Open Session

	if(bSessionOpen)
	{
		ImGui::OpenPopup("Open Session");
	}

//	if(file_dialog.showOpenFileDialog("Open Session", ImVec2(700, 310), ".vistella"))
//	{
//		LOGINFOF("MainWindow::Open file %s", file_dialog.selected_fn.c_str());
//	}

	ImGui::End(); 
}
