// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <vector>
#include "symbolwindow.h"
#include "../imgui/imgui.h"
#include "../symbolstore.h"
#include "../system/imguihelpers.h"
#include "../theme.h"
#include "../system/command.h"

SymbolWindow::SymbolWindow()
{
}

SymbolWindow::~SymbolWindow()
{
}

void SymbolWindow::Draw(bool* pShow)
{
	std::vector<Symbol> symbols = pStore->GetAll();
	
	ImGui::Begin("Symbols", pShow);
	
	// edit subwindow

	if(editingSymbol)
	{
		char buffer[128];
		strcpy(&buffer[0], symbolBeingEdited.labelName.c_str());
		ImGui::InputText("label", buffer, 64);
		symbolBeingEdited.labelName = buffer;
		pStore->UpdateSymbol(symbolBeingEdited);
		
		ImGui::SameLine();
		if(ImGui::Button("Set"))
		{
			Command refreshCmd;
			refreshCmd.name = "refreshDisassembly";
			CommandCenter::Instance()->Broadcast(refreshCmd);
		}
	}

	ImGui::Separator();
	
	// end edit subwindow
	ImGui::BeginChild("SymbolsChild");
	int id = 0;
	for(auto symbol : symbols)
	{
		ImGui::PushID(id++);
		if(ImGui::Button("Edit"))
		{
			editingSymbol = true;
			symbolBeingEdited = symbol;
		}
		ImGui::SameLine();
		
		ImGui::Text("%04x", symbol.address);
		if(symbol.flags & SymbolStore::kSymbolFlag_AddressLabel)
		{
			ImGui::SameLine();
			ImGui::Text("LABEL:");	  
			ImGui::SameLine();
			ImGui::TextColored(kColourLabel, "%s", symbol.labelName.c_str());	  
		}
		if(symbol.flags & SymbolStore::kSymbolFlag_MemoryRead)
		{
			ImGui::SameLine();
			ImGui::Text("READ:");	  
			ImGui::SameLine();
			ImGuiHelpers::TextTooltip(symbol.readName, symbol.readDescription, kColourMemoryRead);
		}
		if(symbol.flags & SymbolStore::kSymbolFlag_MemoryWrite)
		{
			ImGui::SameLine();
			ImGui::Text("WRITE:");	  
			ImGui::SameLine();
			ImGuiHelpers::TextTooltip(symbol.writeName, symbol.writeDescription, kColourMemoryWrite);
		}
		ImGui::PopID();
	}
	ImGui::EndChild();
	ImGui::End();
}

void SymbolWindow::SetSymbolStore(SymbolStore* p)
{
	pStore = p;
}
