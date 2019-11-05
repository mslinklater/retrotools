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
	
	for(int address = 0x0000f000 ; address<0x0000f7ff ; address+=16)
	{
		char buffer[128];
		Memory::MemoryLine line = pMemory->GetLineForAddress((uint16_t)address);
		sprintf(buffer, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
				line.address,
				line.value[0], line.value[1], line.value[2], line.value[3],
				line.value[4], line.value[5], line.value[6], line.value[7],
				line.value[8], line.value[9], line.value[10], line.value[11],
				line.value[12], line.value[13], line.value[14], line.value[15],
				line.key);
		ImGui::Text(buffer);
	}
	
	ImGui::End();
}
