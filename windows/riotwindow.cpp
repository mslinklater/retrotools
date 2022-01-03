// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "riotwindow.h"
#include "common.h"

RiotWindow::RiotWindow()
: pRiot(nullptr)
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
	ImGui::Text("Timer:%d", pRiot->GetTimerVal());
	ImGui::Text("Interval:%d", pRiot->GetTimerInterval());
	ImGui::Text("Delay:%d", pRiot->GetTimerDelay());
	ImGui::Separator();
	ImGui::Columns(4, NULL, true);

	DrawRegister(Riot::kSWCHA, " SWCHA", pRiot->DbgRead(Riot::kSWCHA));
	ImGui::NextColumn();
	DrawRegister(Riot::kSWACNT, "SWACNT", pRiot->DbgRead(Riot::kSWACNT));
	ImGui::NextColumn();
	DrawRegister(Riot::kSWCHB, " SWCHB", pRiot->DbgRead(Riot::kSWCHB));
	ImGui::NextColumn();
	DrawRegister(Riot::kSWBCNT, "SWBCNT", pRiot->DbgRead(Riot::kSWBCNT));

	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Columns(4, NULL, true);

	DrawRegister(Riot::kINTIM, " INTIM", pRiot->DbgRead(Riot::kINTIM));
	ImGui::NextColumn();
	DrawRegister(Riot::kTIMINT, "TIMINT", pRiot->DbgRead(Riot::kTIMINT));
	ImGui::NextColumn();
	DrawRegister(Riot::kTIM1T, " TIM1T", pRiot->DbgRead(Riot::kTIM1T));
	ImGui::NextColumn();
	DrawRegister(Riot::kTIM8T, " TIM8T", pRiot->DbgRead(Riot::kTIM8T));

	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Columns(2, NULL, true);

	DrawRegister(Riot::kTIM64T, "TIM64T", pRiot->DbgRead(Riot::kTIM64T));
	ImGui::NextColumn();
	DrawRegister(Riot::kT1024T, "T1024T", pRiot->DbgRead(Riot::kT1024T));
//	ImGui::NextColumn();
//	DrawRegister(Riot::kTIM1T, " TIM1T", pRiot->DbgRead(Riot::kTIM1T));
//	ImGui::NextColumn();
//	DrawRegister(Riot::kTIM8T, " TIM8T", pRiot->DbgRead(Riot::kTIM8T));

	ImGui::Columns(1);
	ImGui::Separator();
}

void RiotWindow::SetRiot(Riot* riot)
{
	pRiot = riot;
}

void RiotWindow::SerialiseState(json& object)
{

}

void RiotWindow::DeserialiseState(json& object)
{
	
}

