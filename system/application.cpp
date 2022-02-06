// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "application.h"


Application* Application::Instance()
{
	static Application* pInstance = nullptr;

	if(pInstance == nullptr)
	{
		pInstance = new Application();
	}
	return pInstance;
}

Application::Application()
{

}

Application::~Application()
{
	
}

void Application::Init(int argc, char* argv[])
{

}

void Application::UpdateLoop()
{

}

int Application::Close()
{
	return 0;
}

