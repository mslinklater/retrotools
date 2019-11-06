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
			ImGui::Text("%s", line.address.c_str());
			ImGui::SameLine();
			ImGui::Text("%s", line.bytes.c_str());
			ImGui::SameLine();
			ImGui::Text("%s", line.mnemonic.c_str());
			ImGui::SameLine();
			ImGui::Text("%s", line.detail.c_str());
		}
	}
	ImGui::End();
}

void DisassemblyWindow::SetDisassembler(Disassembler* pDisasmIn)
{
	pDisasm = pDisasmIn;
}
