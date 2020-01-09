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
	
void TiaWindow::Draw(void)
{
    ImGui::Text("Write registers");
    ImGui::Separator();
	ImGui::Text("Ticks: %" PRId64, pTia->GetTicksSinceBoot());
    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" VSYNC:0x%02x", pTia->GetVSYNC());
    ImGui::NextColumn();
    ImGui::Text("VBLANK:0x%02x", pTia->GetVBLANK());
    ImGui::NextColumn();
    ImGui::Text(" WSYNC:0x%02x", pTia->GetWSYNC());
    ImGui::NextColumn();
    ImGui::Text(" RSYNC:0x%02x", pTia->GetRSYNC());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("NUSIZ0:0x%02x", pTia->GetNUSIZ0());
    ImGui::NextColumn();
    ImGui::Text("NUSIZ1:0x%02x", pTia->GetNUSIZ1());
    ImGui::NextColumn();
    ImGui::Text("COLUP0:0x%02x", pTia->GetCOLUP0());
    ImGui::NextColumn();
    ImGui::Text("COLUP1:0x%02x", pTia->GetCOLUP1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("COLUPF:0x%02x", pTia->GetCOLUPF());
    ImGui::NextColumn();
    ImGui::Text("COLUBK:0x%02x", pTia->GetCOLUBK());
    ImGui::NextColumn();
    ImGui::Text(" REFP0:0x%02x", pTia->GetREFP0());
    ImGui::NextColumn();
    ImGui::Text(" REFP1:0x%02x", pTia->GetREFP1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("   PF0:0x%02x", pTia->GetPF0());
    ImGui::NextColumn();
    ImGui::Text("   PF1:0x%02x", pTia->GetPF1());
    ImGui::NextColumn();
    ImGui::Text("   PF2:0x%02x", pTia->GetPF2());
    ImGui::NextColumn();
    ImGui::Text(" RESP0:0x%02x", pTia->GetRESP0());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" RESP1:0x%02x", pTia->GetRESP1());
    ImGui::NextColumn();
    ImGui::Text(" RESM0:0x%02x", pTia->GetRESM0());
    ImGui::NextColumn();
    ImGui::Text(" RESM1:0x%02x", pTia->GetRESM1());
    ImGui::NextColumn();
    ImGui::Text(" RESBL:0x%02x", pTia->GetRESBL());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" AUDC0:0x%02x", pTia->GetAUDC0());
    ImGui::NextColumn();
    ImGui::Text(" AUDC1:0x%02x", pTia->GetAUDC1());
    ImGui::NextColumn();
    ImGui::Text(" AUDF0:0x%02x", pTia->GetAUDF0());
    ImGui::NextColumn();
    ImGui::Text(" AUDF1:0x%02x", pTia->GetAUDF1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" AUDV0:0x%02x", pTia->GetAUDV0());
    ImGui::NextColumn();
    ImGui::Text(" AUDV1:0x%02x", pTia->GetAUDV1());
    ImGui::NextColumn();
    ImGui::Text("  GRP0:0x%02x", pTia->GetGRP0());
    ImGui::NextColumn();
    ImGui::Text("  GRP1:0x%02x", pTia->GetGRP1());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" ENAM0:0x%02x", pTia->GetENAM0());
    ImGui::NextColumn();
    ImGui::Text(" ENAM1:0x%02x", pTia->GetENAM1());
    ImGui::NextColumn();
    ImGui::Text(" ENABL:0x%02x", pTia->GetENABL());
    ImGui::NextColumn();
    ImGui::Text("  HMP0:0x%02x", pTia->GetHMP0());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("  HMP1:0x%02x", pTia->GetHMP1());
    ImGui::NextColumn();
    ImGui::Text("  HMM0:0x%02x", pTia->GetHMM0());
    ImGui::NextColumn();
    ImGui::Text("  HMM1:0x%02x", pTia->GetHMM1());
    ImGui::NextColumn();
    ImGui::Text("  HMBL:0x%02x", pTia->GetHMBL());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("VDELP0:0x%02x", pTia->GetVDELP0());
    ImGui::NextColumn();
    ImGui::Text("VDELP1:0x%02x", pTia->GetVDELP1());
    ImGui::NextColumn();
    ImGui::Text("VDELBL:0x%02x", pTia->GetVDELBL());
    ImGui::NextColumn();
    ImGui::Text("RESMP0:0x%02x", pTia->GetRESMP0());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("RESMP1:0x%02x", pTia->GetRESMP1());
    ImGui::NextColumn();
    ImGui::Text(" HMOVE:0x%02x", pTia->GetHMOVE());
    ImGui::NextColumn();
    ImGui::Text(" HMCLR:0x%02x", pTia->GetHMCLR());
    ImGui::NextColumn();
    ImGui::Text(" CXCLR:0x%02x", pTia->GetCXCLR());
    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::Text("Read registers");
    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" CXM0P:0x%02x", pTia->GetCXM0P());
    ImGui::NextColumn();
    ImGui::Text(" CXM1P:0x%02x", pTia->GetCXM1P());
    ImGui::NextColumn();
    ImGui::Text("CXP0FB:0x%02x", pTia->GetCXP0FB());
    ImGui::NextColumn();
    ImGui::Text("CXP1FB:0x%02x", pTia->GetCXP1FB());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text("CXM0FB:0x%02x", pTia->GetCXM0FB());
    ImGui::NextColumn();
    ImGui::Text("CXM1FB:0x%02x", pTia->GetCXM1FB());
    ImGui::NextColumn();
    ImGui::Text("CXBLPF:0x%02x", pTia->GetCXBLPF());
    ImGui::NextColumn();
    ImGui::Text("CXPPMM:0x%02x", pTia->GetCXPPMM());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" INPT0:0x%02x", pTia->GetINPT0());
    ImGui::NextColumn();
    ImGui::Text(" INPT1:0x%02x", pTia->GetINPT1());
    ImGui::NextColumn();
    ImGui::Text(" INPT2:0x%02x", pTia->GetINPT2());
    ImGui::NextColumn();
    ImGui::Text(" INPT3:0x%02x", pTia->GetINPT3());
    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Columns(4, NULL, true);
    ImGui::Text(" INPT4:0x%02x", pTia->GetINPT4());
    ImGui::NextColumn();
    ImGui::Text(" INPT5:0x%02x", pTia->GetINPT5());
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