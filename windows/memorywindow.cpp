// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "memorywindow.h"
#include "../imgui/imgui.h"
#include "../components/memory.h"

MemoryWindow::MemoryWindow()
: startAddress(0)
, length(1024)
, startAddressText("0000")
, lengthText("0400")
{
}

MemoryWindow::~MemoryWindow()
{
}

void MemoryWindow::SetMemory(Memory* mem)
{
	pMemory = mem;
}

void MemoryWindow::Draw(void)
{
	if(pMemory == nullptr)
	{
		ImGui::Text("ERROR - Memory not set");
		ImGui::End();
		return;
	}
	
	{
		ImGui::PushItemWidth(40);
		ImGui::InputText("Start", startAddressText, 5, ImGuiInputTextFlags_CharsHexadecimal);
		ImGui::SameLine();
		ImGui::InputText("Length", lengthText, 5, ImGuiInputTextFlags_CharsHexadecimal);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		
		if(ImGui::Button("Update"))
		{
			// update 
			startAddress = (int)strtol(startAddressText, NULL, 16);
			length = (int)strtol(lengthText, NULL, 16);
		}
		
		ImGui::Separator();
	}
	
	ImGui::BeginChild("ScrollArea");
	{
		// num lines of memory
		uint16_t numLines = (length + 15) / 16;

		ImGuiListClipper clipper;
		clipper.Begin(numLines);
		while(clipper.Step())
		{
			for(int iLine = clipper.DisplayStart ; iLine < clipper.DisplayEnd ; iLine++)
			{
				int address = startAddress + (iLine * 16);
				
				ImGui::Text("%04x", address);

				for(int i=0 ; i<16 ; i++)
				{
					ImVec4 col(0.75, 0.75, 0.75, 1.0);
					uint8_t flags = pMemory->GetFlag(address+i);

					if(flags & Memory::kMemoryFlagReadFrom)
					{
						col.y += 0.25f;
					}
					if(flags & Memory::kMemoryFlagWrittenTo)
					{
						col.x += 0.25f;
					}

					ImGui::SameLine();
					ImGui::TextColored(col, "%02x", pMemory->Read(address+i, false));
				}
				for(int i=0 ; i<16 ; i++)
				{
					ImGui::SameLine();
					ImGui::Text("%c", pMemory->Read(address+i, false));
				}			
			}
		}
	}
	ImGui::EndChild();
}

