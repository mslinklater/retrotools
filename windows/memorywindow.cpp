#include "memorywindow.h"
#include "../imgui/imgui.h"
#include "../memory.h"

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

void MemoryWindow::Draw()
{
	ImGui::Begin("Memory");
	
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
		for(int address = startAddress ; (address < startAddress+length) && (address<0x0000ffff) ; address+=16)
		{
			ImGui::Text("%04x", address);

			for(int i=0 ; i<16 ; i++)
			{
				ImGui::SameLine();
				ImGui::Text("%02x", pMemory->Read(address+i));
			}
			for(int i=0 ; i<16 ; i++)
			{
				ImGui::SameLine();
				ImGui::Text("%c", pMemory->Read(address+i));
			}			
		}
	}
	ImGui::EndChild();
	
	ImGui::End();
}

/*
void Memory::PopulateLines()
{
	lines.clear();
	for(int location = 0 ; location < MEMORY_SIZE ; location += 16)
	{
		MemoryLine newLine;
		
		// Address
		sprintf(&(newLine.address[0]), "%04x", (uint16_t)location);
		
		// Bytes
		for(int iByte=0 ; iByte<16 ; iByte++)
		{
			sprintf(&(newLine.value[iByte][0]), "%02x", pMemory[location+iByte]);
		}
		
		// Keys
		for(int iByte=0 ; iByte<16 ; iByte++)
		{
			newLine.key[iByte] = pMemory[location+iByte];
		}
		newLine.key[16] = 0;
		lines.push_back(newLine);
	}
}
*/
