// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "windowmanager.h"
#include "windowbase.h"
#include "../log.h"
#include "../imgui/imgui.h"
#include "../commands.h"

WindowManager::WindowManager()
: initialised(false)
, receivedQuit(false)
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
	CommandCenter::Instance()->Subscribe("Quit", this);
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
	if(command.name == Commands::kToggleWindowCommand)
	{
		windowActive[command.payload] = !windowActive[command.payload];
	}
	if(command.name == Commands::kQuitCommand)
	{
		// save the state of the windows
		receivedQuit = true;
	}
	
	return false;
}

bool WindowManager::ReceivedQuit()
{
	return receivedQuit;
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
