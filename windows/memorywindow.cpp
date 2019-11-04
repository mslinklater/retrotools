#include "memorywindow.h"
#include "../imgui/imgui.h"
#include "../memory.h"

MemoryWindow::MemoryWindow()
: startAddress(0)
{
}

MemoryWindow::~MemoryWindow()
{
}

void MemoryWindow::SetMemory(Memory* mem)
{
	pMemory = mem;
}

void MemoryWindow::Draw()
{
	ImGui::Begin("Memory");
	
	if(pMemory == nullptr)
	{
		ImGui::Text("ERROR - Memory not set");
		ImGui::End();
		return;
	}
	
	for(int address = 0xf000 ; address<0xf100 ; address+=16)
	{
		Memory::MemoryLine line = pMemory->GetLineForAddress((uint16_t)address);
		
	}
	
	ImGui::End();
}
