// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <vector>
#include "symbolwindow.h"
#include "../imgui/imgui.h"
#include "../symbolstore.h"

SymbolWindow::SymbolWindow()
{
}

SymbolWindow::~SymbolWindow()
{
}

void SymbolWindow::Draw(bool* pShow)
{
	std::vector<SymbolStore::Symbol> symbols = pStore->GetAll();
	
	ImGui::Begin("Symbols", pShow);
	for(size_t i=0 ; i<symbols.size() ; i++)
	{
		ImGui::Text("%04x", symbols[i].address);
		ImGui::SameLine();
		ImGui::Text("%s ", symbols[i].name.c_str());
	}
	ImGui::End();
}

void SymbolWindow::SetSymbolStore(SymbolStore* p)
{
	pStore = p;
}
