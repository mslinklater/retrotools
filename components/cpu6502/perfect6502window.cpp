// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "perfect6502window.h"
#include "common.h"

Perfect6502Window::Perfect6502Window()
:bRunning(false)
{
	pState = initAndResetChip();
}

Perfect6502Window::~Perfect6502Window()
{
	destroyChip(pState);
}

void Perfect6502Window::Draw(void)
{
	if(ImGui::Button(bRunning ? "Stop" : "Start"))
	{
		bRunning = !bRunning;
	}

	if(bRunning)
	{
		step(pState);
	}
	else
	{

	}
}

void Perfect6502Window::SerialiseState(json& object)
{

}

void Perfect6502Window::DeserialiseState(json& object)
{
	
}
