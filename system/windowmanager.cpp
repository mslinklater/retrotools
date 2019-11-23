// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "windowmanager.h"
#include "windowbase.h"
#include "../log.h"
#include "../imgui/imgui.h"

WindowManager::WindowManager()
: initialised(false)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::Init()
{
	LOGINFO("WindowManager::Init");
	
	// Subscribe to all ToggleWindow commands
	CommandCenter::Instance()->Subscribe("ToggleWindow", this);
	initialised = true;
}

void WindowManager::Draw()
{
	assert(initialised == true);
	
	for(auto window : windows)
	{
		std::string name = window.first;
		bool active = windowActive[name];
		if(active)
		{
			ImGui::Begin(name.c_str(), &active);
			WindowBase* pWindow = window.second;
			pWindow->Draw();
			ImGui::End();
			windowActive[name] = active;
		}
	}
}

bool WindowManager::HandleCommand(const Command& command)
{
	windowActive[command.payload] = !windowActive[command.payload];
	
	return false;
}

eErrorCode WindowManager::AddWindow(WindowBase* pWindow, std::string name)
{
	assert(initialised == true);
	
	if(windowActive.find(name) == windowActive.end())
	{
		// new window
		windows[name] = pWindow;
		windowActive[name] = false;
		windowNames.push_back(name);
	}
	
	return kError_OK;
}

const std::vector<std::string> WindowManager::GetWindows()
{
	return windowNames;
}
