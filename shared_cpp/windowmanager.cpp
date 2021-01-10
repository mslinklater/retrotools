// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "windowmanager.h"
#include "windowbase.h"
#include "log.h"
#include "../imgui/imgui.h"	// external dependency
#include "command.h"
#include "stateserialiser.h"


WindowManager::WindowManager()
: initialised(false)
, receivedQuit(false)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::Init(StateSerialiser* pStateSerialiser)
{
	LOGINFO("WindowManager::Init");
	
	// Subscribe to all ToggleWindow commands
	CommandCenter::Instance()->Subscribe(SharedCommands::kToggleWindowCommand, this);
	CommandCenter::Instance()->Subscribe(SharedCommands::kQuitCommand, this);

	pStateSerialiser->AddStateSerialiser(this);
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
			const char* thisWindow = name.c_str();
			ImGui::Begin(thisWindow, &active);
			WindowBase* pWindow = window.second;
			pWindow->Draw();
			ImGui::End();
			windowActive[name] = active;
		}
	}
}

bool WindowManager::HandleCommand(const Command& command)
{
	if(command.name == SharedCommands::kToggleWindowCommand)
	{
		windowActive[command.payload] = !windowActive[command.payload];
	}
	if(command.name == SharedCommands::kQuitCommand)
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

void WindowManager::SerialiseState(json& object)
{
	LOGINFO("WindowManager::SerialiseState");

	json windowsJson = json::array();
	for(auto window : windows)
	{
		json windowJson = json::object();
		windowJson["name"] = window.first.c_str();
		bool active = windowActive[window.first];
		windowJson["active"] = active;
		windowsJson.push_back( windowJson );
	}
	object["window_manager"] = windowsJson;
}

void WindowManager::DeserialiseState(json& object)
{
	LOGINFO("WindowManager::DeserialiseState");

	json windowsJson = object["window_manager"];
	if(windowsJson.is_array())
	{
		for(auto window : windowsJson)
		{
			std::string windowName = window["name"].get<std::string>();
			windowActive[windowName] = window["active"].get<bool>();
		}
	}
}
