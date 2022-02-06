// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <SDL.h>
#include <SDL_opengl.h>
#include "system/buildconfig.h"

#if RUN_TESTS
#define CATCH_CONFIG_RUNNER
#include "shared_cpp/catch.hpp"
#endif

#include <iostream>

#include "system/common.h"
#include "system/application.h"

#include "3rdparty/imgui/imgui_impl_sdl.h"
#include "3rdparty/imgui/imgui_impl_opengl2.h"

#include "system/lua/luavm.h"

#include "version.h"
#include "components/stella/memory2600.h"
#include "components/cpu6502/cpu6502alpha.h"
#include "components/stella/tia.h"
#include "components/stella/riot.h"
#include "disasm.h"
#include "symbolstore.h"
#include "system/commands.h"
#include "windows/logwindow.h"
#include "windows/helpaboutwindow.h"
#include "windows/helpcommandswindow.h"
#include "windows/resourceswindow.h"
#include "components/stella/memory2600window.h"
#include "windows/disasmwindow.h"
#include "windows/mainwindow.h"
#include "components/cpu6502/cpu6502window.h"
#include "components/cpu6502/perfect6502window.h"
#include "system/commandcenter.h"
#include "system/window/windowmanager.h"
#include "system/stateserialiser.h"
#include "machines/machinebase.h"

// DearImGui stuff
static uint64_t	performanceFrequency;
static uint64_t	performanceCounterLast;
static SDL_GLContext gl_context;
static SDL_Window* window;
static int displayWidth;
static int displayHeight;

int InitImGui()
{
	char titleString[64];
	sprintf(titleString, "Retrotools V%d.%d", Retrotools_VERSION_MAJOR, Retrotools_VERSION_MINOR);

	LOGINFO(titleString);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		LOGERRORF("%s\n", SDL_GetError());
		return -1;
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

	return 0;
}

void ShutdownImGui()
{
	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void OutOfMemoryHandler()
{
	std::cerr << "Out of memory!";
	exit(-1);
}

int main(int argc, char* argv[])
{
	Application::Instance()->Init(argc, argv);

	std::set_new_handler(OutOfMemoryHandler);

	// TODO: Output the command line to stdout

#if RUN_TESTS
	int result = Catch::Session().run(argc, argv);
	if(result != 0)
	{
		LOGINFO("Unit tests failed...");
		return result;
	}
#endif

	// check for command line args
	
	// SDL stuff
	if(InitImGui() != 0)
	{
		return -1;
	}

	// Init Lua VM

	LuaVM *pLua = new LuaVM();
	delete pLua;

	ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

	// Initialise window manager

	std::shared_ptr<StateSerialiser> pStateSerialiser(new StateSerialiser());
	std::shared_ptr<WindowManager> pWindowManager(new WindowManager());

	pWindowManager->Init(pStateSerialiser);
	pStateSerialiser->AddStateSerialiser(pWindowManager);

	// create windows

	// Log Window
	std::shared_ptr<LogWindow> pLogWindow(new LogWindow());
	pWindowManager->AddWindow(pLogWindow, "Log");
	pStateSerialiser->AddStateSerialiser(pLogWindow);

	std::shared_ptr<HelpAboutWindow> pHelpAboutWindow(new HelpAboutWindow());
	pWindowManager->AddWindow(pHelpAboutWindow, "HelpAbout");

	std::shared_ptr<HelpCommandsWindow> pHelpCommandsWindow(new HelpCommandsWindow());
	pWindowManager->AddWindow(pHelpCommandsWindow, "HelpCommands");

	std::shared_ptr<ResourcesWindow> pResourcesWindow(new ResourcesWindow());
	pWindowManager->AddWindow(pResourcesWindow, "Resources");

	std::shared_ptr<MainWindow> pMainWindow(new MainWindow());
	pMainWindow->SetWindowManager(pWindowManager.get());
	
	pStateSerialiser->DeserialiseAppConfig();

	bool done = false;

#if IMGUI_DEMO	
	bool show_demo_window = false;
#endif

	Application::Instance()->UpdateLoop();

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
		glClearColor(clear_color.x, clear_color.y,clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		
		if(pWindowManager->ReceivedQuit())
		{
			done = true;
		}
	}

	// Save the state of the windows
	pStateSerialiser->SerialiseAppConfig();

	ShutdownImGui();
	
	return Application::Instance()->Close();
}
