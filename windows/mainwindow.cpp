// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <algorithm>
#include <filesystem>

#include "mainwindow.h"
#include "system/common.h"
#include "system/message/messagecenter.h"
#include "system/message/messages.h"
#include "system/window/windowmanager.h"
#include "filebrowser/ImGuiFileBrowser.h"
#include "system/message/messagehelpers.h"
#include "system/usercommands.h"
#include "projectconfig.h"
#include "system/formatting.h"

//imgui_addons::ImGuiFileBrowser file_dialog; // As a class member or globally

MainWindow::MainWindow()
: open(true)
, bShowNewSession(false)
, bScrollToBottom(false)
, commandHistoryPtr(0)
{
	memset(&inputBuffer[0], 0, kInputBufferSize);
}

MainWindow::~MainWindow()
{
	LOGINFO("MainWindow::~MainWindow()");
}

void MainWindow::Init(const MainWindowInit& init)
{
	init.messageCenter->Subscribe(TextOutputMessage::kName, this);
	init.messageCenter->Subscribe(ScrollToBottomMessage::kName, this);
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
				MessageHelpers::Quit();
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
					MessageHelpers::ToggleWindow(name);
				}
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Help"))
		{
			if(ImGui::MenuItem("Commands"))
			{
				MessageHelpers::ToggleWindow("HelpCommands");
			}
			if(ImGui::MenuItem("About"))
			{
				MessageHelpers::ToggleWindow("HelpAbout");
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
		std::vector<std::string> matches;
		std::string commandBuffer(data->Buf);
		UserCommands::Instance()->GetCompletions(commandBuffer, matches);

		// update the command prompt
		strncpy(data->Buf, commandBuffer.c_str(), data->BufSize);
		data->BufTextLen = commandBuffer.length();
		data->CursorPos = data->BufTextLen;
		data->BufDirty = true;

		// display the matches
		for(const auto &item : matches)
		{
			MessageHelpers::TextOutput(item);
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
		if(item[0] == '[')
		{
			// some sort of formatting
			if(outputItems[i].find(TextFormat::kBoldTag) == 0)
			{
				ImGui::TextColored(ImVec4(1.0,1.0,1.0,1.0),"%s", item + TextFormat::kBoldTagSize);
			}
			else
			{
				ImGui::TextColored(ImVec4(0.6,0.6,0.6,1.0),"%s", item);
			}
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

	ImGui::Text("%s>", std::filesystem::current_path().string().c_str());
	ImGui::SameLine();
	if (ImGui::InputText(" ", inputBuffer, kInputBufferSize, inputTextFlags, &CommandPromptCallbackStub, (void *)this))
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
			delete [] newBuffer;

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

IMessageHandler::Return MainWindow::HandleMessage(const std::shared_ptr<MessageBase> command)
{
	if(command->name == TextOutputMessage::kName)
	{
		std::shared_ptr<TextOutputMessage> cmd = std::dynamic_pointer_cast<TextOutputMessage>(command);
		outputItems.push_back(cmd->text);
		while(outputItems.size() > ProjectConfig::kMaxOutputLines)
		{
			outputItems.erase(outputItems.begin());
		}
	}
	if(command->name == ScrollToBottomMessage::kName)
	{
		bScrollToBottom = true;
	}
	return IMessageHandler::kForward;
}
