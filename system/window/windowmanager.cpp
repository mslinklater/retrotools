// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "system/common.h"
#include "windowmanager.h"
#include "windowbase.h"
#include "system/commands.h"
#include "system/stateserialiser.h"
#include "resources/resourcemanager.h"
#include "windows/resources/t64window.h"

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
	CommandCenter::Instance()->Subscribe(CloseResourceWindowCommand::kName, this);

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

	if(command->name == CloseResourceWindowCommand::kName)
	{
		std::shared_ptr<CloseResourceWindowCommand> cmd = std::dynamic_pointer_cast<CloseResourceWindowCommand>(command);
		std::string windowName = std::string("Resource-") + cmd->resourceId;

		auto res = windows.find(windowName);
		if (res != windows.end())
		{
			RemoveWindow(windowName);
		}
	}

	if(command->name == OpenResourceWindowCommand::kName)
	{
		std::shared_ptr<OpenResourceWindowCommand> cmd = std::dynamic_pointer_cast<OpenResourceWindowCommand>(command);
		std::string windowName = std::string("Resource-") + cmd->resourceId;

		// check for window already existing

		if(windows.find(windowName) != windows.end())
		{
			ShowWindow(windowName, true);
		}
		else
		{
			ResourceManager::EResourceType resourceType = ResourceManager::Instance()->GetResourceType(cmd->resourceId);

			std::shared_ptr<WindowBase> newWindow(nullptr);

			switch(resourceType)
			{
				case ResourceManager::EResourceType::T64File:
				{
					std::shared_ptr<ResourceBase> res = ResourceManager::Instance()->GetResourcePtr(cmd->resourceId);
					T64Window *pWindow = new T64Window();
					pWindow->pResource = res;
					newWindow = std::shared_ptr<WindowBase>(pWindow);
					break;				
				}
				case ResourceManager::EResourceType::D64File:
					break;
				case ResourceManager::EResourceType::Unknown:
				default:
					LOGERRORF("Cannot open window for resource %s", cmd->resourceId.c_str());
					return false;
			}

			if(newWindow != nullptr)
			{
				AddWindow(newWindow, windowName);
				ShowWindow(windowName, true);
				ResourceManager::Instance()->SetResourceWindow(cmd->resourceId, windowName);
			}
		}
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

eErrorCode WindowManager::RemoveWindow(std::string name)
{
	if(windowActive.find(name) != windowActive.end())
	{
		windowActive.erase(name);
		windows.erase(name);

		for(std::vector<std::string>::iterator i = windowNames.begin() ; i != windowNames.end() ; i++)
		{
			if(*i == name)
			{
				windowNames.erase(i);
				break;
			}
		}
	}
	return kError_OK;
}

eErrorCode WindowManager::ShowWindow(std::string name, bool bShow)
{
	if(windowActive.find(name) != windowActive.end())
	{
		windowActive[name] = bShow;
		return kError_OK;
	}
	return kError_WindowNotFound;
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
