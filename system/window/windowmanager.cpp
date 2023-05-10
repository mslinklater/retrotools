// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include <sstream>

#include "system/common.h"
#include "windowmanager.h"
#include "windowbase.h"
#include "system/message/messages.h"
#include "system/stateserialiser.h"
#include "system/lua/luavm.h"
#include "resources/resourcemanager.h"
#include "windows/resources/t64window.h"
#include "windows/resources/binarywindow.h"

static WindowManager* luaContext_WindowManager = nullptr;

WindowManager::WindowManager()
: initialised(false)
, receivedQuit(false)
{
}

WindowManager::~WindowManager()
{
	LOGINFO("WindowManager::~WindowManager()");
}

static int lua_WindowCreate( lua_State* pState )
{
	LUA_FUNCDEF("window_create");
	LUA_ASSERT_NUMPARAMS(2);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);	// type
	LUA_ASSERT_TYPE(2, LUA_TSTRING);	// identifier

	if(luaContext_WindowManager->ShowWindow(lua_tostring(pState, 1), true) == kError_OK)
	{
		lua_pushboolean(pState, true);
	}
	else
	{
		lua_pushboolean(pState, false);
	}
	return 1;
}

void WindowManager::Init(const WindowManagerInit& init)
{
	LOGINFO("WindowManager::Init");
	
	messageCenter = init.messageCenter;

	// Subscribe to all ToggleWindow commands
	messageCenter->Subscribe(ToggleWindowMessage::kName, this);
	messageCenter->Subscribe(QuitMessage::kName, this);
	messageCenter->Subscribe(OpenResourceWindowMessage::kName, this);
	messageCenter->Subscribe(CloseResourceWindowMessage::kName, this);

	// Register Lua functions
	luaContext_WindowManager = this;
	init.lua->RegisterCFunction(lua_WindowCreate, "window_create");


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

IMessageHandler::Return WindowManager::HandleMessage(const std::shared_ptr<MessageBase> command)
{
	if(command->name == ToggleWindowMessage::kName)
	{
		std::shared_ptr<ToggleWindowMessage> cmd = std::dynamic_pointer_cast<ToggleWindowMessage>(command);
		if(windows.find(cmd->windowName) != windows.end())
		{
			windowActive[cmd->windowName] = !windowActive[cmd->windowName];
		}
		else
		{
			LOGWARNINGF("WindowManager::ToggleWindow - cannot find window named '%s'", cmd->windowName.c_str());
		}
		return IMessageHandler::kConsumed;
	}

	if(command->name == CloseResourceWindowMessage::kName)
	{
		std::shared_ptr<CloseResourceWindowMessage> cmd = std::dynamic_pointer_cast<CloseResourceWindowMessage>(command);
		std::string windowName = std::string("Resource-") + cmd->resourceId;

		auto res = windows.find(windowName);
		if (res != windows.end())
		{
			RemoveWindow(windowName);
		}
		return IMessageHandler::kConsumed;
	}

	if(command->name == OpenResourceWindowMessage::kName)
	{
		std::shared_ptr<OpenResourceWindowMessage> cmd = std::dynamic_pointer_cast<OpenResourceWindowMessage>(command);
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
				case ResourceManager::EResourceType::Binary:
				{
					std::shared_ptr<ResourceBase> res = ResourceManager::Instance()->GetResourcePtr(cmd->resourceId);
					BinaryWindow *pWindow = new BinaryWindow();
					pWindow->pResource = res;
					newWindow = std::shared_ptr<WindowBase>(pWindow);
				}
				case ResourceManager::EResourceType::Unknown:
					break;
				default:
					LOGERRORF("Cannot open window for resource %s", cmd->resourceId.c_str());
					break;
			}

			if(newWindow != nullptr)
			{
				AddWindow(newWindow, windowName);
				ShowWindow(windowName, true);
				ResourceManager::Instance()->SetResourceWindow(cmd->resourceId, windowName);
			}
		}
		return IMessageHandler::kConsumed;
	}

	if(command->name == QuitMessage::kName)
	{
		// save the state of the windows
		receivedQuit = true;
	}

	return IMessageHandler::kForward;
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
	LOGWARNINGF("WindowManager::ShowWindow Window not found %s", name.c_str());
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


//void WindowManager::RegisterLuaInterface(std::shared_ptr<LuaVM> pLua)
//{
//	pLua->RegisterCFunction(lua_WindowCreate, "window_create");
//}
