// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "disasmwindow.h"
#include "../imgui/imgui.h"
#include "../disasm.h"
#include "../components/cpu6502.h"
#include "../log.h"

DisassemblyWindow::DisassemblyWindow()
: pDisasm(0)
, showTIAHints(false)
, followPC(false)
{
}

DisassemblyWindow::~DisassemblyWindow()
{
}

void DisassemblyWindow::DrawHeader(void)
{
	ImGui::Checkbox("TIA hints", &showTIAHints);
	ImGui::Checkbox("Follow PC", &followPC);
}

void DisassemblyWindow::DrawMainSubWindow(void)
{
	ImGui::BeginChild("MainSub");
	int numLines = pDisasm->GetNumLines();
	for(int i=0 ; i<numLines ; i++)
	{
		Disassembler::Line line = pDisasm->GetLine(i);
		
		if(line.label.length() > 0)
		{
			ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s:", line.label.c_str());
		}
		if(showTIAHints)
		{
			ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), " TIA ");
		}
		else
		{	
			if(pCpu->GetPC() == line.address)
			{
				ImGui::Text("==>  ");	   
			}
			else
			{
				ImGui::Text("     ");
			}
		}
		ImGui::SameLine();
		ImGui::Text("    %s", line.addressString.c_str());
		ImGui::SameLine();
		ImGui::Text("%s", line.bytes.c_str());
		ImGui::SameLine();
		ImGui::Text("%s", line.mnemonicString.c_str());
		ImGui::SameLine();
		if(line.flags & Disassembler::kFlagSymbol)
		{
			ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "%s", line.detail.c_str());
		}
		else if(line.flags & Disassembler::kFlagLabel)
		{
			ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", line.detail.c_str());	   
		}
		else
		{
			ImGui::Text("%s", line.detail.c_str());
		}
	}
	ImGui::EndChild();
}

void DisassemblyWindow::SetCPU(Cpu6502* cpu)
{
	pCpu = cpu;
}

void DisassemblyWindow::Draw()
{
	DrawHeader();
	
	ImGui::Separator();
	
	if(pDisasm == nullptr)
	{
		ImGui::Text("ERROR - Disasm not set");
	}
	else
	{
		DrawMainSubWindow();
	}
}

void DisassemblyWindow::SetDisassembler(Disassembler* pDisasmIn)
{
	pDisasm = pDisasmIn;
}

void DisassemblyWindow::SerialiseState(json& object)
{
	LOGINFO("DisassemblyWindow::SerialiseState");
}

void DisassemblyWindow::DeserialiseState(json& object)
{
	LOGINFO("DisassemblyWindow::DeserialiseState");
}
