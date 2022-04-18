// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "application.h"
#include "buildconfig.h"

#include "3rdparty/imgui/imgui_impl_sdl.h"
#include "3rdparty/imgui/imgui_impl_opengl2.h"

#include "system/common.h"

#include "version.h"
#include "system/lua/luavm.h"
#include "system/stateserialiser.h"
#include "system/window/windowmanager.h"
#include "windows/logwindow.h"
#include "windows/luawindow.h"
#include "windows/helpaboutwindow.h"
#include "windows/helpcommandswindow.h"
#include "windows/resourceswindow.h"
#include "windows/mainwindow.h"

#include "machines/machine_simple6502.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <filesystem>

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

static int lua_LoadFile( lua_State* pState )
{
	LUA_FUNCDEF("loadfile");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);

	std::string filename(lua_tostring(pState, -1));
	Application::Instance()->GetLua()->LoadScript(filename);

	return 0;
}

static int lua_Cwd( lua_State* pState )
{
	LUA_FUNCDEF("cwd");
	LUA_ASSERT_NUMPARAMS(0);

	lua_pushstring(pState, std::filesystem::current_path().string().c_str() );
	return 1;
}

static int lua_Cd( lua_State* pState )
{
	LUA_FUNCDEF("cd");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);

	const char* path = lua_tostring(pState, 1);

    std::filesystem::path newPath = std::filesystem::current_path() /= path;
    if(std::filesystem::exists(newPath))
    {
        std::filesystem::current_path(newPath);
	}

	lua_pushstring(pState, std::filesystem::current_path().string().c_str() );
	return 1;
}

static int lua_Ls( lua_State* pState )
{
	LUA_FUNCDEF("ls");
	LUA_ASSERT_NUMPARAMS(0);

	lua_newtable(pState);
//	int table = lua_gettop(pState);
//	lua_settable(pState, table);
    return 1;
}

static int lua_MachineCreate( lua_State* pState )
{
	LUA_FUNCDEF("machine_create");
	LUA_ASSERT_NUMPARAMS(2);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);
	LUA_ASSERT_TYPE(2, LUA_TSTRING);

	std::shared_ptr<MachineBase> pMachine;
	const std::string machineType(lua_tostring(pState, 1));
	const std::string machineName(lua_tostring(pState, 2));

	if(machineType == "simple6502")
	{
		pMachine = std::make_shared<MachineSimple6502>();
	}
	else
	{
		LOGERRORF("Unable to create machine of type %s", machineType.c_str());
		return 0;
	}

	pMachine->SetName(machineName);

	// TODO - this will need work to support multiple machines
	Application::Instance()->SetMachine(pMachine);

	return 0;
}

void OutOfMemoryHandler()
{
	std::cerr << "Out of memory!";
	exit(-1);
}

void Application::Init(int argc, char* argv[])
{
	std::set_new_handler(OutOfMemoryHandler);

	InitImGui();

	// Bring up the Lua system
	pLua = std::make_shared<LuaVM>();
	pLua->Init();
	pLua->RegisterCFunction(lua_LoadFile, "loadfile");
	pLua->RegisterCFunction(lua_Cwd, "cwd");
	pLua->RegisterCFunction(lua_Cd, "cd");
	pLua->RegisterCFunction(lua_Ls, "ls");
	pLua->RegisterCFunction(lua_MachineCreate, "machine_create");

	pStateSerialiser = std::make_shared<StateSerialiser>();

	pWindowManager = std::make_shared<WindowManager>();
//	pWindowManager = WindowManager::Instance();
	pWindowManager->Init(pStateSerialiser);
	pWindowManager->RegisterLuaInterface(pLua);
	pStateSerialiser->AddStateSerialiser(pWindowManager);

	pLogWindow = std::make_shared<LogWindow>();
	pWindowManager->AddWindow(pLogWindow, "Log");
	pStateSerialiser->AddStateSerialiser(pLogWindow);

	pLuaWindow = std::make_shared<LuaWindow>();
	pLuaWindow->Init(pLua);
	pWindowManager->AddWindow(pLuaWindow, "Lua");

	pHelpAboutWindow = std::make_shared<HelpAboutWindow>();
	pWindowManager->AddWindow(pHelpAboutWindow, "HelpAbout");

	pHelpCommandsWindow = std::make_shared<HelpCommandsWindow>();
	pWindowManager->AddWindow(pHelpCommandsWindow, "HelpCommands");

	pResourcesWindow = std::make_shared<ResourcesWindow>();
	pWindowManager->AddWindow(pResourcesWindow, "Resources");

	pMainWindow = std::make_shared<MainWindow>();
	pMainWindow->SetWindowManager(pWindowManager);
	
	pStateSerialiser->DeserialiseAppConfig();

	pLua->LoadScript("../lua/postinit.lua");
}

void Application::InitImGui()
{
	char titleString[64];
	sprintf(titleString, "Retrotools V%d.%d", Retrotools_VERSION_MAJOR, Retrotools_VERSION_MINOR);

	LOGINFO(titleString);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		LOGERRORF("%s\n", SDL_GetError());
		return;
	}

	performanceFrequency = SDL_GetPerformanceFrequency();
	performanceCounterLast = SDL_GetPerformanceCounter();

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);	
	window = SDL_CreateWindow(	titleString, 
											SDL_WINDOWPOS_CENTERED, 
											SDL_WINDOWPOS_CENTERED, 
											640, 480, windowFlags);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();

	displayWidth = io.DisplaySize.x;
	displayHeight = io.DisplaySize.y;
}

void Application::ShutdownImGui()
{
	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::UpdateLoop()
{
	bool done = false;

#if IMGUI_DEMO	
	bool show_demo_window = false;
#endif

	while(!done)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		uint64_t performanceCounterThis = SDL_GetPerformanceCounter();
		float dt = ((float)(performanceCounterThis - performanceCounterLast) / (float)performanceFrequency);
		if(dt < 0.0f) dt = 0.0f;
		performanceCounterLast = performanceCounterThis;

		//pSystem->Update(dt);
		CommandCenter::Instance()->Update();
		
		// start ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		
		// do stuff
#if IMGUI_DEMO
		ImGui::ShowDemoWindow(&show_demo_window);
#endif

		pWindowManager->Draw();	// draw all managed windows		
		pMainWindow->Draw();
		
		// rendering
		ImGui::Render();
		glViewport(0, 0, displayWidth, displayHeight);
		glClearColor(kClearColour.x, kClearColour.y, kClearColour.z, kClearColour.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		
		if(pWindowManager->ReceivedQuit())
		{
			done = true;
		}
	}
}

int Application::Close()
{
	// Save the state of the windows
	pStateSerialiser->SerialiseAppConfig();

	return 0;
}

void Application::SetMachine(std::shared_ptr<MachineBase> _pMachine)
{
	pMachine = _pMachine;
}
