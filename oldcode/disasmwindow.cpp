// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#if 0
#include "disasmwindow.h"
#include "system/common.h"
#include "../disasm.h"
#include "../components/cpu6502/cpu6502alpha.h"
#include "components/imemory.h"

DisassemblyWindow::DisassemblyWindow()
: pDisasm(0)
, followPC(false)
{
}

DisassemblyWindow::~DisassemblyWindow()
{
}

void DisassemblyWindow::DrawHeader(void)
{
	ImGui::Checkbox("Follow PC", &followPC);
	if(ImGui::Button("Add Auto Symbols"))
	{
		pDisasm->AddObviousLabels();
	}
	ImGui::SameLine();
	if(ImGui::Button("Remove Auto Symbols"))
	{
		pDisasm->DeleteAutoSymbols();
	}
}

void DisassemblyWindow::DrawMainSubWindow(void)
{
	ImGui::BeginChild("MainSub");
	int numLines = pDisasm->GetNumLines();

	if(followPC)
	{
		uint32_t pcline = 0;
		uint32_t setTo = 0;
		for(int i=0 ; i<numLines && pcline == 0 ; i++)
		{
			Disassembler::Line line = pDisasm->GetLine(i);
			if(pCpu->GetPC() == line.address)
			{
				pcline = setTo;
			}
			setTo++;
		}
		if(pcline != 0)
		{
			int32_t offset = pcline * (ImGui::GetFontSize()+4) - 50;
			if(offset <0) offset = 0;
			ImGui::SetScrollY(offset);
		}
	}

	ImGuiListClipper clipper;
	clipper.Begin(numLines);
	while(clipper.Step())
	{
		for(int i = clipper.DisplayStart ; i < clipper.DisplayEnd ; i++)
		{
			Disassembler::Line line = pDisasm->GetLine(i);
			
			if(line.label.length() > 0)
			{
				ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s:", line.label.c_str());
			}
			else
			{
				ImGui::Text(" ");
			}

			// highlight PC		
			if(pCpu->GetPC() == line.address)
			{
				ImGui::SameLine(170.0f);
				ImGui::Text("==>");	   
			}

			ImGui::SameLine(200.0f);
			if(pCpu->IsBreakpoint(line.address))
			{
				ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "%s", line.addressString.c_str());
			}
			else
			{
				ImGui::Text("%s", line.addressString.c_str());
			}
			
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
	}
	clipper.End();
	ImGui::EndChild();
}

void DisassemblyWindow::SetCPU(Cpu6502Base* cpu)
{
	pCpu = cpu;
}

void DisassemblyWindow::SetMemory(IMemory* pMem)
{
	pMemory = pMem;
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
#endif