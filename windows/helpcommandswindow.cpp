// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "common.h"
#include "helpcommandswindow.h"
#include "version.h"

HelpCommandsWindow::HelpCommandsWindow()
: userCommands(UserCommands::Instance()->GetCommandInfo())
{
}

HelpCommandsWindow::~HelpCommandsWindow()
{
}

void HelpCommandsWindow::DrawCommandsForType(UserCommands::Type t)
{
	for(auto command : userCommands)
	{
		if(command.type == t)
		{
			ImGui::Separator();
			ImGui::Text(command.hint.c_str(), 0);
			ImGui::Text(" ");
			for(const std::string& line : command.helpText)
			{
				ImGui::Text(line.c_str(), 0);
			}
		}
	}
	ImGui::Separator();		
}

void HelpCommandsWindow::Draw(void)
{
    if (ImGui::CollapsingHeader("General..."))
	{
		DrawCommandsForType(UserCommands::EGeneral);
	}
    if (ImGui::CollapsingHeader("Files..."))
	{
		DrawCommandsForType(UserCommands::EFileOperation);
	}
}
