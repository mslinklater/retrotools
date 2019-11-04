#include "logwindow.h"
#include "../log.h"
#include "../imgui/imgui.h"

LogWindow::LogWindow()
: showInfo(true)
, showWarnings(true)
, showErrors(true)
{
}

LogWindow::~LogWindow()
{
}

void LogWindow::Draw()
{
	ImGui::Begin("Log");
	
	ImGui::Checkbox("Info", &showInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Warnings", &showWarnings);
	ImGui::SameLine();
	ImGui::Checkbox("Errors", &showErrors);
	
	ImGui::Separator();

	ImGui::BeginChild("output", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
	
	int numLines = Log::Instance()->GetLineCount();
	for(int iLine = 0 ; iLine < numLines ; iLine++)
	{
		Log::LogLine line = Log::Instance()->GetLine(iLine);
		switch(line.type)
		{
			case Log::eLogType::kInfo:
				ImGui::TextColored(ImVec4(1.0,1.0,1.0,1.0),line.content.c_str());
				break;
			case Log::eLogType::kWarning:
				ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0),line.content.c_str());
				break;
			case Log::eLogType::kError:
				ImGui::TextColored(ImVec4(1.0,0.0,0.0,1.0),line.content.c_str());
				break;
		}
	}
	
	ImGui::EndChild();
	
	ImGui::End();
}
