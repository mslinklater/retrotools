// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "common.h"
#include "windowmanager.h"
#include "windowbase.h"
#include "system/commands.h"
#include "system/stateserialiser.h"


WindowManager::WindowManager()
: initialised(false)
, receivedQuit(false)
{
}

WindowManager::~WindowManager()
{
	LOGINFO("WindowManager::~WindowManager()");
}

void WindowManager::Init(std::shared_ptr<StateSerialiser> pStateSerialiser)
{
	LOGINFO("WindowManager::Init");
	
	// Subscribe to all ToggleWindow commands
	CommandCenter::Instance()->Subscribe(ToggleWindowCommand::kName, this);
	CommandCenter::Instance()->Subscribe(QuitCommand::kName, this);
	CommandCenter::Instance()->Subscribe(OpenResourceWindowCommand::kName, this);

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
			WindowBase* pWindow = window.second.get();
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

	if(command->name == OpenResourceWindowCommand::kName)
	{
		std::shared_ptr<OpenResourceWindowCommand> cmd = std::dynamic_pointer_cast<OpenResourceWindowCommand>(command);
		std::string windowName = std::string("Resource-") + cmd->resourceId;
		
	}

	if(command->name == QuitCommand::kName)
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

eErrorCode WindowManager::AddWindow(std::shared_ptr<WindowBase> pWindow, std::string name)
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
