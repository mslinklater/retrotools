// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <cassert>
#include "windowmanager.h"

WindowManager::WindowManager()
: initialised(false)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::Init()
{
	initialised = true;
}

void WindowManager::Draw()
{
	assert(initialised == true);
}

bool WindowManager::HandleCommand(const Command& command)
{
	return false;
}

eErrorCode WindowManager::AddWindow(WindowBase* pWindow, std::string name)
{
	assert(initialised == true);
	
	return kError_OK;
}
