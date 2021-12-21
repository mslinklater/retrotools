// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "common.h"
#include "windowmanager.h"
#include "windowbase.h"
#include "commands.h"
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
	CommandCenter::Instance()->Subscribe(ToggleWindowCommand::kName, this);
//	CommandCenter::Instance()->Subscribe(kQuitCommand, this);

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

bool WindowManager::HandleCommand(const std::shared_ptr<CommandBase> command)
{
	if(command->name == ToggleWindowCommand::kName)
	{
		std::shared_ptr<ToggleWindowCommand> cmd = std::dynamic_pointer_cast<ToggleWindowCommand>(command);
		if(windows.find(cmd->windowName) != windows.end())
		{
			windowActive[cmd->windowName] = !windowActive[cmd->windowName];
		}
		else
		{
			LOGWARNINGF("WindowManager::ToggleWindow - cannot find window named '%s'", cmd->windowName.c_str());
		}
	}
#if 0
	if(command->name == kQuitCommand)
	{
		// save the state of the windows
		receivedQuit = true;
	}
#endif
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
