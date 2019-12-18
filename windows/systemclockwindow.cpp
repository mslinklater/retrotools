// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "systemclockwindow.h"
#include "../components/systemclock.h"
#include "../imgui/imgui.h"

SystemClockWindow::SystemClockWindow()
{

}

SystemClockWindow::~SystemClockWindow()
{

}

void SystemClockWindow::SetSystemClock(SystemClock* clock)
{
    pClock = clock;
}

void SystemClockWindow::Draw(void)
{
    ImGui::Text("TIA");
    if(ImGui::Button("Tick"))
    {

    }
    ImGui::SameLine();
    if(ImGui::Button("HBlank"))
    {

    }
    ImGui::SameLine();
    if(ImGui::Button("VBlank"))
    {

    }
    ImGui::Separator();
    ImGui::Text("CPU");
    if(ImGui::Button("Tick"))
    {

    }
    ImGui::SameLine();
    if(ImGui::Button("Instruction"))
    {

    }
}

