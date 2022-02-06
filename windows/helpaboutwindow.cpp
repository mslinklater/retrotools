// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "system/common.h"
#include "helpaboutwindow.h"
#include "version.h"

HelpAboutWindow::HelpAboutWindow()
{

}

HelpAboutWindow::~HelpAboutWindow()
{
}

void HelpAboutWindow::Draw(void)
{
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "--== Retrotools %d.%d (BUILD %s %s)==--", Retrotools_VERSION_MAJOR, Retrotools_VERSION_MINOR, __DATE__, __TIME__);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), " ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "BSD 2-Clause License");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), " ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "Copyright (c) 2019-2021, Martin Linklater");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "All rights reserved.");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), " ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "Redistribution and use in source and binary forms, with or without");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "modification, are permitted provided that the following conditions are met:");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), " ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "1. Redistributions of source code must retain the above copyright notice, this");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "   list of conditions and the following disclaimer.");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), " ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "2. Redistributions in binary form must reproduce the above copyright notice,");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "   this list of conditions and the following disclaimer in the documentation");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "   and/or other materials provided with the distribution.");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), " ");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS'");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75f), "OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
	
}

void HelpAboutWindow::SerialiseState(json& object)
{
	LOGINFO("HelpAboutWindow::SerialiseState");
}

void HelpAboutWindow::DeserialiseState(json& object)
{
	LOGINFO("HelpAboutWindow::DeserialiseState");
}
