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
		ImGui::TextUnformatted(Log::Instance()->GetLine(iLine).c_str());
	}
	
	ImGui::EndChild();
	
	ImGui::End();
}
