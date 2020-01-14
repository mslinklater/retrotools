// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "tiawindow.h"
#include "../imgui/imgui.h"
#include "../components/tia.h"

TiaWindow::TiaWindow()
{

}

TiaWindow::~TiaWindow()
{

}

void TiaWindow::DrawWriteRegister(uint32_t reg, const char* text, uint8_t val)
{
	bool temp = pTia->GetWriteBreakpoint(reg);
	bool backup = temp;
	ImGui::Checkbox(text, &temp);
	if(temp != backup)
	{
		pTia->SetWriteBreakpoint(reg, temp);
	}
	ImGui::SameLine();
    ImGui::Text(":0x%02x", val);
}

void TiaWindow::DrawReadRegister(uint32_t reg, const char* text, uint8_t val)
{
	bool temp = pTia->GetReadBreakpoint(reg);
	bool backup = temp;
	ImGui::Checkbox(text, &temp);
	if(temp != backup)
	{
		pTia->SetReadBreakpoint(reg, temp);
	}
	ImGui::SameLine();
    ImGui::Text(":0x%02x", val);
}

void TiaWindow::Draw(void)
{
    ImGui::Text("Write registers");
    ImGui::Separator();
	ImGui::Text("Ticks: %" PRId64, pTia->GetTicksSinceBoot());
	ImGui::Text("RasterX: %d", pTia->GetRasterX());
	ImGui::Text("RasterY: %d", pTia->GetRasterY());
	ImGui::Text("Frame: %d", pTia->GetFrameNum());
    ImGui::Separator();
    ImGui::Columns(4, NULL, true);

	DrawWriteRegister(Tia::kVSYNC, " VSYNC", pTia->GetVSYNC());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kVBLANK, "VBLANK", pTia->GetVBLANK());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kWSYNC, " WSYNC", pTia->GetWSYNC());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kRSYNC, " RSYNC", pTia->GetRSYNC());

    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kNUSIZ0, "NUSIZ0", pTia->GetNUSIZ0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kNUSIZ1, "NUSIZ1", pTia->GetNUSIZ1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kCOLUP0, "COLUP0", pTia->GetCOLUP0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kCOLUP1, "COLUP1", pTia->GetCOLUP1());

    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kCOLUPF, "COLUPF", pTia->GetCOLUPF());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kCOLUBK, "COLUBK", pTia->GetCOLUBK());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kREFP0, " REFP0", pTia->GetREFP0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kREFP1, " REFP1", pTia->GetREFP1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kPF0, "   PF0", pTia->GetPF0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kPF1, "   PF1", pTia->GetPF1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kPF2, "   PF2", pTia->GetPF2());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kRESP0, " RESP0", pTia->GetRESP0());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kRESP1, " RESP1", pTia->GetRESP1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kRESM0, " RESM0", pTia->GetRESM0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kRESM1, " RESM1", pTia->GetRESM1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kRESBL, " RESBL", pTia->GetRESBL());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kAUDC0, " AUDC0", pTia->GetAUDC0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kAUDC1, " AUDC1", pTia->GetAUDC1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kAUDF0, " AUDF0", pTia->GetAUDF0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kAUDF1, " AUDF1", pTia->GetAUDF1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kAUDV0, " AUDV0", pTia->GetAUDV0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kAUDV1, " AUDV1", pTia->GetAUDV1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kGRP0, "  GRP0", pTia->GetGRP0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kGRP1, "  GRP1", pTia->GetGRP1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kENAM0, " ENAM0", pTia->GetENAM0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kENAM1, " ENAM1", pTia->GetENAM1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kENABL, " ENABL", pTia->GetENABL());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kHMP0, "  HMP1", pTia->GetHMP0());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kHMP1, "  HMP1", pTia->GetHMP1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kHMM0, "  HMM0", pTia->GetHMM0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kHMM1, "  HMM1", pTia->GetHMM1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kHMBL, "  HMBL", pTia->GetHMBL());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kVDELP0, "VDELP0", pTia->GetVDELP0());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kVDELP1, "VDELP1", pTia->GetVDELP1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kVDELBL, "VDELBL", pTia->GetVDELBL());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kRESMP0, "RESMP0", pTia->GetRESMP0());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawWriteRegister(Tia::kRESMP1, "RESMP1", pTia->GetRESMP1());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kHMOVE, " HMOVE", pTia->GetHMOVE());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kHMCLR, " HMCLR", pTia->GetHMCLR());
    ImGui::NextColumn();
	DrawWriteRegister(Tia::kCXCLR, " CXCLR", pTia->GetCXCLR());
    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::Text("Read registers");
    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawReadRegister(Tia::kCXM0P, " CXM0P", pTia->GetCXM0P());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kCXM1P, " CXM1P", pTia->GetCXM1P());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kCXP0FB, "CXP0FB", pTia->GetCXP0FB());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kCXP1FB, "CXP1FB", pTia->GetCXP1FB());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawReadRegister(Tia::kCXM0FB, "CXM0FB", pTia->GetCXM0FB());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kCXM1FB, "CXM1FB", pTia->GetCXM1FB());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kCXBLPF, "CXBLPF", pTia->GetCXBLPF());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kCXPPMM, "CXPPMM", pTia->GetCXPPMM());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawReadRegister(Tia::kINPT0, " INPT0", pTia->GetINPT0());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kINPT1, " INPT1", pTia->GetINPT1());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kINPT2, " INPT2", pTia->GetINPT2());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kINPT3, " INPT3", pTia->GetINPT3());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
	DrawReadRegister(Tia::kINPT4, " INPT4", pTia->GetINPT4());
    ImGui::NextColumn();
	DrawReadRegister(Tia::kINPT5, " INPT5", pTia->GetINPT5());
    ImGui::NextColumn();
    ImGui::Text(" ");
    ImGui::NextColumn();
    ImGui::Text(" ");
    ImGui::Columns(1);
    ImGui::Separator();

    // Draw screen image
}

void TiaWindow::SetTia(Tia* tia)
{
    pTia = tia;
}