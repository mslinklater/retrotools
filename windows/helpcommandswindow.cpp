// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "common.h"
#include "helpcommandswindow.h"
#include "version.h"

HelpCommandsWindow::HelpCommandsWindow()
{

}

HelpCommandsWindow::~HelpCommandsWindow()
{
}

void HelpCommandsWindow::Draw(void)
{
    if (ImGui::CollapsingHeader("General..."))
	{
		ImGui::Separator();
		ImGui::Text("> help <command>");
		ImGui::Text(" ");
		ImGui::Text("Prints help about the specified command");

		ImGui::Separator();
		ImGui::Text("> quit");
		ImGui::Text(" ");
		ImGui::Text("Quit the application");

		ImGui::Separator();
	}
    if (ImGui::CollapsingHeader("Files..."))
	{
		ImGui::Separator();
		ImGui::Text("> open <filename>");
		ImGui::Text(" ");
		ImGui::Text("Open file");

		ImGui::Separator();
		ImGui::Text("> pwd");
		ImGui::Text(" ");
		ImGui::Text("Print current working directory");

		ImGui::Separator();
	}

}
