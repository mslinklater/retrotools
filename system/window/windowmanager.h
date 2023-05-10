// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <map>
#include "system/message/messagecenter.h"
#include "system/errorcodes.h"
#include "system/istateserialisation.h"

class WindowBase;
class StateSerialiser;
class LuaVM;

struct WindowManagerInit
{
	std::shared_ptr<LuaVM> lua;
	std::shared_ptr<MessageCenter> messageCenter;
};

class WindowManager : public IMessageHandler, public IStateSerialisation
{
public:
	WindowManager();
	virtual ~WindowManager();

	eErrorCode AddWindow(std::shared_ptr<WindowBase> pWindow, std::string name);
	eErrorCode ShowWindow(std::string name, bool bShow);
	eErrorCode RemoveWindow(std::string name);

	const std::vector<std::string> GetWindows();
	
	void Init(const WindowManagerInit& init);
	void RegisterLuaInterface(std::shared_ptr<LuaVM> pLua);
	void Draw();
	bool ReceivedQuit();

	ISERIALISATION_API

private:
	
	IMESSAGEHANDLER_API;
	
	std::shared_ptr<MessageCenter> messageCenter;

	bool initialised;

	// TODO: Combine these two into a struct
	std::map<std::string,std::shared_ptr<WindowBase>> windows;
	std::map<std::string,bool> windowActive;

	std::vector<std::string> windowNames;	// TODO: Can we get rid of this and just use the map keys ?
	bool receivedQuit;
};
