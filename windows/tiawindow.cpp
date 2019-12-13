// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "tiawindow.h"
#include "../imgui/imgui.h"

TiaWindow::TiaWindow()
{

}

TiaWindow::~TiaWindow()
{

}
	
void TiaWindow::Draw(void)
{
    ImGui::Text("Write registers");
//    ImGui::Text("VSYNC:0x%02x", pTia->GetVSYNC());
}

void TiaWindow::SetTia(Tia* tia)
{
    pTia = tia;
}