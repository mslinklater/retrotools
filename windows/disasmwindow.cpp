#include "disasmwindow.h"
#include "../imgui/imgui.h"
#include "../disasm.h"

DisassemblyWindow::DisassemblyWindow()
: pDisasm(0)
{
}

DisassemblyWindow::~DisassemblyWindow()
{
}

void DisassemblyWindow::Draw()
{
	ImGui::Begin("Disassembly");
	if(pDisasm == nullptr)
	{
		ImGui::Text("ERROR - Disasm not set");
	}
	else
	{
		int numLines = pDisasm->GetNumLines();
		for(int i=0 ; i<numLines ; i++)
		{
			Disassembler::Line line = pDisasm->GetLine(i);
			if(line.label.length() > 0)
			{
				ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", line.label.c_str());
			}
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
	}
	ImGui::End();
}

void DisassemblyWindow::SetDisassembler(Disassembler* pDisasmIn)
{
	pDisasm = pDisasmIn;
}
