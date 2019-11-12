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

void MemoryWindow::Draw(bool* pOpen)
{
	ImGui::Begin("Memory", pOpen);
	
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
