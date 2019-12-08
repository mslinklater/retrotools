// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "memorywindow.h"
#include "../imgui/imgui.h"
#include "../components/memory.h"

MemoryWindow::MemoryWindow()
{
}

MemoryWindow::~MemoryWindow()
{
}

void MemoryWindow::SetMemory(Memory* mem)
{
	pMemory = mem;
}

void MemoryWindow::DrawLine(uint16_t startAddress)
{
	ImGui::Text("%04x", startAddress);

	for(int i=0 ; i<16 ; i++)
	{
		ImVec4 col(0.5, 0.5, 0.5, 1.0);
		uint8_t flags = pMemory->GetFlag(startAddress+i);

		if(flags & Memory::kMemoryFlagReadFrom)
		{
			col.y = 1.0f;
		}
		if(flags & Memory::kMemoryFlagWrittenTo)
		{
			col.z += 1.0f;
		}
		if((flags & Memory::kMemoryFlagWriteBreakpoint) || (flags & Memory::kMemoryFlagReadBreakpoint))
		{
			col.x += 1.0f;
		}

		ImGui::SameLine();
		ImGui::TextColored(col, "%02x", pMemory->Read(startAddress+i, false));
	}
}

void MemoryWindow::Draw(void)
{
	if(pMemory == nullptr)
	{
		ImGui::Text("ERROR - Memory not set");
		ImGui::End();
		return;
	}

	// RAM 0x80-0xff
	{
		ImGui::Text("RAM");
		for(int iLine = 0; iLine < 8 ; iLine++)
		{
			int address = 0x0080 + (iLine * 16);

			DrawLine(address);
		}
	}

	ImGui::Separator();

	// ROM 0xf000 - 0xffff
	ImGui::Text("ROM");
	ImGui::BeginChild("ScrollArea");
	{
		// num lines of memory
		uint16_t numLines = (0x1000 + 15) / 16;

		ImGuiListClipper clipper;
		clipper.Begin(numLines);
		while(clipper.Step())
		{
			for(int iLine = clipper.DisplayStart ; iLine < clipper.DisplayEnd ; iLine++)
			{
				int address = 0xf000 + (iLine * 16);

				DrawLine(address);
			}
		}
	}
	ImGui::EndChild();
}

