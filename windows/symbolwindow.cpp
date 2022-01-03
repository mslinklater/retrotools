// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#include <vector>
#include "symbolwindow.h"
#include "common.h"
#include "../symbolstore.h"
#include "../utils/imguihelpers.h"
#include "../theme.h"
#include "system/commandcenter.h"

SymbolWindow::SymbolWindow()
: filterFlags(0)
, filterMemoryRead(false)
, filterMemoryWrite(false)
, filterAddressLabel(false)
, filterHardware(false)
, filterUser(false)
, filterAuto(false)
, filterJumpLabel(false)
, filterBranchLabel(false)
{
}

SymbolWindow::~SymbolWindow()
{
}

void SymbolWindow::Draw()
{
	std::vector<Symbol> symbols = pStore->GetAll();
	
	// filter

	ImGui::Text("Filters");
	ImGui::Checkbox("Label", &filterAddressLabel);
	ImGui::Checkbox("Auto", &filterAuto);
	ImGui::Checkbox("Branch Label", &filterBranchLabel);
	ImGui::Checkbox("Hardware", &filterHardware);
	ImGui::Checkbox("Jump Label", &filterJumpLabel);
	ImGui::Checkbox("Memory Read", &filterMemoryRead);
	ImGui::Checkbox("Memory Write", &filterMemoryWrite);
	ImGui::Checkbox("User", &filterUser);
	
	// build the flags filter
	filterFlags = 0;
	if(filterAddressLabel) filterFlags |= Symbol::kSymbolFlag_AddressLabel;
	if(filterAuto) filterFlags |= Symbol::kSymbolFlag_Auto;
	if(filterBranchLabel) filterFlags |= Symbol::kSymbolFlag_BranchLabel;
	if(filterHardware) filterFlags |= Symbol::kSymbolFlag_Hardware;
	if(filterJumpLabel) filterFlags |= Symbol::kSymbolFlag_JumpLabel;
	if(filterMemoryRead) filterFlags |= Symbol::kSymbolFlag_MemoryRead;
	if(filterMemoryWrite) filterFlags |= Symbol::kSymbolFlag_MemoryWrite;
	if(filterUser) filterFlags |= Symbol::kSymbolFlag_User;
	
	ImGui::Separator();
	
	// edit subwindow

	if(editingSymbol)
	{
		char buffer[128];
		strcpy(&buffer[0], symbolBeingEdited.labelName.c_str());
		ImGui::InputText("label", buffer, 64);
		symbolBeingEdited.labelName = buffer;
		symbolBeingEdited.flags |= Symbol::kSymbolFlag_User;
		pStore->UpdateSymbol(symbolBeingEdited);
		
		ImGui::SameLine();
		if(ImGui::Button("Set"))
		{
#if 0
			Command refreshCmd;
			refreshCmd.name = "refreshDisassembly";
			CommandCenter::Instance()->QueueForBroadcast(std::make_shared<Command>(refreshCmd));
#endif
		}
		ImGui::Separator();
	}

	
	// end edit subwindow
	ImGui::BeginChild("SymbolsChild");
	int id = 0;
	for(auto symbol : symbols)
	{
		if(symbol.flags & filterFlags)
		{
			ImGui::PushID(id++);
			if(ImGui::Button("Edit"))
			{
				editingSymbol = true;
				symbolBeingEdited = symbol;
			}
			ImGui::SameLine();
			
			ImGui::Text("%04x", symbol.address);
			if(symbol.flags & Symbol::kSymbolFlag_AddressLabel)
			{
				ImGui::SameLine();
				ImGui::Text("LABEL:");	  
				ImGui::SameLine();
				ImGui::TextColored(kColourLabel, "%s", symbol.labelName.c_str());	  
			}
			if(symbol.flags & Symbol::kSymbolFlag_MemoryRead)
			{
				ImGui::SameLine();
				ImGui::Text("READ:");	  
				ImGui::SameLine();
				ImGuiHelpers::TextTooltip(symbol.readName, symbol.readDescription, kColourMemoryRead);
			}
			if(symbol.flags & Symbol::kSymbolFlag_MemoryWrite)
			{
				ImGui::SameLine();
				ImGui::Text("WRITE:");	  
				ImGui::SameLine();
				ImGuiHelpers::TextTooltip(symbol.writeName, symbol.writeDescription, kColourMemoryWrite);
			}
			ImGui::PopID();
		}
	}
	ImGui::EndChild();
}

void SymbolWindow::SetSymbolStore(SymbolStore* p)
{
	pStore = p;
}
