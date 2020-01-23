#include "riotwindow.h"
#include "../imgui/imgui.h"
#include "../shared_cpp/log.h"

RiotWindow::RiotWindow()
{

}

RiotWindow::~RiotWindow()
{

}

void RiotWindow::DrawRegister(uint32_t reg, const char* text, uint8_t val)
{
	bool temp = pRiot->GetBreakpoint(reg);
	bool backup = temp;
	ImGui::Checkbox(text, &temp);
	if(temp != backup)
	{
		pRiot->SetBreakpoint(reg, temp);
	}
	ImGui::SameLine();
    ImGui::Text(":0x%02x", val);
}

void RiotWindow::Draw(void)
{

}

void RiotWindow::SetRiot(Riot* riot)
{

}

void RiotWindow::SerialiseState(json& object)
{

}

void RiotWindow::DeserialiseState(json& object)
{
	
}

