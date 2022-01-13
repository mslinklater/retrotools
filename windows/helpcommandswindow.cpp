// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "common.h"
#include "helpcommandswindow.h"
#include "version.h"
#include "system/formatting.h"

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
			if(command.helpText.size() > 0)
			{
				ImGui::Text(" ");
				for(const std::string& line : command.helpText)
				{
					ImGui::TextColored(TextFormat::kColourNormal, line.c_str(), 0);
				}
			}
		}
	}
	ImGui::Separator();		
}

void HelpCommandsWindow::Draw(void)
{
    if (ImGui::CollapsingHeader("General..."))
	{
		DrawCommandsForType(UserCommands::Type::EGeneral);
	}
    if (ImGui::CollapsingHeader("Files & File System..."))
	{
		DrawCommandsForType(UserCommands::Type::EFileOperation);
	}
}
