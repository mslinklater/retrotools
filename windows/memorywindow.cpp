#include "memorywindow.h"
#include "../imgui/imgui.h"
#include "../memory.h"

MemoryWindow::MemoryWindow()
: startAddress(0)
, length(1024)
, startAddressText("0000")
, lengthText("1000")
{
}

MemoryWindow::~MemoryWindow()
{
}

void MemoryWindow::SetMemory(Memory* mem)
{
	pMemory = mem;
}

void MemoryWindow::Update()
{
	/*
	// Keep start and length values valid
	for(int i=0 ; i<4 ; i++)
	{
		bool valid = false;
		if(startAddressText[i] >= '0' && startAddressText[i] <= '9')
			valid = true;
		if(startAddressText[i] >= 'a' && startAddressText[i] <= 'f')
			valid = true;
		
		if(!valid)
			startAddressText[i] = '0';
	}
	*/
}
/*
static int StartAddressTextCallback(ImGuiInputTextCallbackData* pData)
{
//	((MemoryWindow*)pData)
	printf("Callback!\n");
	return 0;
}
*/
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
	
	for(int address = startAddress ; (address < startAddress+length) && (address<0x0000f7ff) ; address+=16)
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
		ImGui::Text("%s", buffer);
	}
	
	ImGui::End();
}
