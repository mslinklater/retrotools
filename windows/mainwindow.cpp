// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
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
#include "settings.h"

//imgui_addons::ImGuiFileBrowser file_dialog; // As a class member or globally

MainWindow::MainWindow()
: open(true)
, bShowNewSession(false)
, bScrollToBottom(false)
, commandHistoryPtr(0)
{
	memset(&inputBuffer[0], 0, kInputBufferSize);
	CommandCenter::Instance()->Subscribe(TextOutputCommand::kName, this);
	CommandCenter::Instance()->Subscribe(ScrollToBottomCommand::kName, this);
}

MainWindow::~MainWindow()
{
	LOGINFO("MainWindow::~MainWindow()");
}

void MainWindow::DrawMenuBar()
{
	// Main Menu Bar
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Session"))
		{
#if 0
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
#endif
			if(ImGui::MenuItem("Quit"))
			{
				// quit session
				CommandHelpers::Quit();
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
		// only if we actually have a history to go through
		if(commandHistory.size() > 1)
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
	}
	if(data->EventFlag & ImGuiInputTextFlags_CallbackCompletion)	// completion
	{
		// find num matches - if one, call that completion handler... if many, display the options.
		const std::vector<std::string> matches = UserCommands::Instance()->GetCompletions(std::string(data->Buf));
		
		if(matches.size() > 0)
		{
			if(matches.size() == 1)
			{
				// one match - so complete the prompt with the completion
				strncpy(data->Buf, matches[0].c_str(), data->BufSize);
				data->BufTextLen = matches[0].length();
				data->CursorPos = data->BufTextLen;
				data->BufDirty = true;
			}
			else
			{
				// multiple matches
				for(const auto &item : matches)
				{
					CommandHelpers::TextOutput(item);
				}
			}
		}
		else
		{
			// no matches
		}
	}
	return 0;
}

void MainWindow::ClearOutput()
{

}

void MainWindow::DrawConsole()
{
	ImGui::Separator();
	// Scroll region

    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, 0);
	// the actual log
	for(std::size_t i=0; i<outputItems.size(); i++)
	{
		const char* item = outputItems[i].c_str();
		// TODO: Need to add some formatting here
		if(item[0] == '>')
		{
			ImGui::TextColored(ImVec4(1.0,1.0,1.0,1.0),"%s", item);
		}
		else
		{
			ImGui::TextColored(ImVec4(0.6,0.6,0.6,1.0),"%s", item);
		}
	}
	if(bScrollToBottom)
	{
		ImGui::SetScrollHereY(1.0f);
		bScrollToBottom = false;
	}
	ImGui::EndChild();

	ImGui::Separator();
	// Command-Line
	bool reclaimFocus = false;
	ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;

	ImGui::Text(">");
	ImGui::SameLine();
	if (ImGui::InputText("command", inputBuffer, kInputBufferSize, inputTextFlags, &CommandPromptCallbackStub, (void *)this))
	{
		if(strlen(inputBuffer))
		{
			// add to command history
			std::string historyString(&inputBuffer[0]);
			commandHistory.push_back(historyString);

			// add to logItems
			char* newBuffer = new char[strlen(inputBuffer) + 3];
			sprintf(newBuffer, "> %s", inputBuffer);
			std::string newString(&newBuffer[0]);
			outputItems.push_back(newString);
			delete newBuffer;

			UserCommands::Instance()->ParseAndProcessCommand(std::string(inputBuffer));
		}

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

bool MainWindow::HandleCommand(const std::shared_ptr<CommandBase> command)
{
	if(command->name == TextOutputCommand::kName)
	{
		std::shared_ptr<TextOutputCommand> cmd = std::dynamic_pointer_cast<TextOutputCommand>(command);
		outputItems.push_back(cmd->text);
		while(outputItems.size() > Settings::kMaxOutputLines)
		{
			outputItems.erase(outputItems.begin());
		}
	}
	if(command->name == ScrollToBottomCommand::kName)
	{
		bScrollToBottom = true;
	}
	return false;
}
