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

void SymbolWindow::Draw()
{
	std::vector<SymbolStore::Symbol> symbols = pStore->GetAll();
	
	ImGui::Begin("Symbols");
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
