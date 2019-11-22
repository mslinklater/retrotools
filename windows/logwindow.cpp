// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

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

void LogWindow::Draw(bool* pOpen)
{
	ImGui::Begin("Log", pOpen);
	
	// Filter tick boxes
	const std::set<std::string>& categories = Log::Instance()->GetCategories();
	for(std::string category : categories)
	{
		ImGui::Text("%s", category.c_str());
	}
	ImGui::Separator();
	
	ImGui::Checkbox("Info", &showInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Warnings", &showWarnings);
	ImGui::SameLine();
	ImGui::Checkbox("Errors", &showErrors);
	
	ImGui::Separator();
	ImGui::BeginChild("output", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
	
	std::vector<int> lineNumbers;
	int numLines = Log::Instance()->GetLineCount();
	
	for(int i=0 ; i<numLines ; i++)
	{
		Log::LogLine line = Log::Instance()->GetLine(i);
		if(showInfo && line.type == Log::eLogType::kInfo)
		{
			lineNumbers.push_back(i);
		}
		if(showWarnings && line.type == Log::eLogType::kWarning)
		{
			lineNumbers.push_back(i);
		}
		if(showErrors && line.type == Log::eLogType::kError)
		{
			lineNumbers.push_back(i);
		}
	}
	
	ImGuiListClipper clipper;
	clipper.Begin(lineNumbers.size());
	while(clipper.Step())
	{
		for(int iLine = clipper.DisplayStart ; iLine < clipper.DisplayEnd ; iLine++)
		{
			Log::LogLine line = Log::Instance()->GetLine(lineNumbers[iLine]);
			
			if(!line.category.empty())
			{
				ImGui::TextColored(ImVec4(0.0,1.0,0.0,1.0),"%s:", line.category.c_str());
				ImGui::SameLine();
			}
			
			switch(line.type)
			{
				case Log::eLogType::kInfo:
					if(showInfo)
					{
						ImGui::TextColored(ImVec4(1.0,1.0,1.0,1.0),"%s", line.content.c_str());
					}
					break;
				case Log::eLogType::kWarning:
					if(showWarnings)
					{
						ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0),"%s", line.content.c_str());
					}
					break;
				case Log::eLogType::kError:
					if(showErrors)
					{
						ImGui::TextColored(ImVec4(1.0,0.0,0.0,1.0),"%s", line.content.c_str());
					}
					break;
			}
		}
	}
	clipper.End();
	
	ImGui::EndChild();
	
	ImGui::End();
}
