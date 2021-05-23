// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <SDL.h>
#include <SDL_opengl.h>
#include "globalconfig.h"


#if RUN_TESTS
#define CATCH_CONFIG_RUNNER
#include "shared_cpp/catch.hpp"
#endif

#include "common.h"
#include "3rdparty/imgui/imgui_impl_sdl.h"
#include "3rdparty/imgui/imgui_impl_opengl2.h"

#include "version.h"
//#include "config.h"
#include "components/memory2600.h"
#include "components/cpu6502alpha.h"
#include "components/tia.h"
#include "components/riot.h"
#include "components/system.h"
#include "disasm.h"
#include "sessionmanager.h"
#include "symbolstore.h"
#include "commands.h"
#include "windows/logwindow.h"
#include "windows/memory2600window.h"
#include "windows/disasmwindow.h"
#include "windows/mainwindow.h"
#include "windows/symbolwindow.h"
#include "windows/cpu6502window.h"
#include "windows/tiawindow.h"
#include "windows/riotwindow.h"
#include "windows/systemwindow.h"
#include "windows/perfect6502window.h"
#include "command.h"
#include "utils/memoryutils.h"
#include "windowmanager.h"
#include "stateserialiser.h"

//static Config* pConfig = 0;

#if 0
void ProcessCommandLine(int argc, char* argv[])
{
	pConfig = new Config();
	pConfig->Init();
	
	if(pConfig->ParseCommandLine(argc, argv) != kError_OK)
	{
		LOGINFO("Error parsing command line... aborting \n");
		exit(1);
	}
}
#endif

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

int main(int argc, char* argv[])
{
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
	
//	ProcessCommandLine(argc, argv);
	
	// SDL stuff
	if(InitImGui() != 0)
	{
		return -1;
	}

	ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

	// Initialise window manager

	StateSerialiser* pStateSerialiser = new StateSerialiser();
	WindowManager* pWindowManager = new WindowManager();
	pWindowManager->Init(pStateSerialiser);

	// Session Manager
//	SessionManager* pSessionManager = SessionManager::Instance();
//	pSessionManager = nullptr;	// TEMP

	// Do some 6502 stuff
	// initialise components
	
	Memory2600* pMemory = new Memory2600();
	Cpu6502Alpha* pCpu = new Cpu6502Alpha();
	Tia* pTia = new Tia();
	Riot* pRiot = new Riot();

	System* pSystem = new System();
	pSystem->SetTia(pTia);
	pSystem->SetCpu6502Alpha(pCpu);
	pSystem->SetRiot(pRiot);
	pSystem->SetMemory(pMemory);

	pStateSerialiser->AddStateSerialiser(pMemory);
	pStateSerialiser->AddStateSerialiser(pCpu);

	Disassembler* pDisassembler = new Disassembler();
	SymbolStore* pSymbolStore = new SymbolStore();
	pStateSerialiser->AddStateSerialiser(pSymbolStore);
	
	if(pSymbolStore->LoadHardwareFromJSON("vcs_symbols.json") != kError_OK)
	{
		return -1;
	}
	
	pDisassembler->Init();
	pDisassembler->SetMemory(pMemory);
	pDisassembler->SetCpu(pCpu);
	pDisassembler->SetSymbolStore(pSymbolStore);

#if 0
	uint16_t bytesLoaded = 0;
	uint16_t loadAddress = pConfig->GetLoadAddress();

	// need to grab the ROM filename from the config settings
	if(!pConfig->GetLoadFilename().empty())
	{
		if (MemoryUtils::LoadFileToMemory(pMemory, pConfig->GetLoadFilename(), loadAddress, &bytesLoaded) != kError_OK)
		{
			return -1;
		}
	}
#endif

	// TODO: Handle loaded address better
//	pDisassembler->Disassemble(0xf000, bytesLoaded, loadAddress);
	
	// create windows

	// Log Window
	LogWindow* pLogWindow = new LogWindow();
	pWindowManager->AddWindow(pLogWindow, "Log");
	pStateSerialiser->AddStateSerialiser(pLogWindow);
	
	Memory2600Window* pMemoryWindow = new Memory2600Window();
	pMemoryWindow->SetMemory(pMemory);
	pWindowManager->AddWindow(pMemoryWindow, "Memory2600");
	pStateSerialiser->AddStateSerialiser(pMemoryWindow);
	
	DisassemblyWindow* pDisasmWindow = new DisassemblyWindow();
	pDisasmWindow->SetDisassembler(pDisassembler);
	pDisasmWindow->SetCPU(pCpu);
	pDisasmWindow->SetMemory(pMemory);
	pWindowManager->AddWindow(pDisasmWindow, "Disassembly");
	pStateSerialiser->AddStateSerialiser(pDisasmWindow);

	MainWindow* pMainWindow = new MainWindow();
	pMainWindow->SetWindowManager(pWindowManager);
	
	SymbolWindow* pSymbolWindow = new SymbolWindow();
	pSymbolWindow->SetSymbolStore(pSymbolStore);
	pWindowManager->AddWindow(pSymbolWindow, "Symbols");
	
	Cpu6502Window* pCpu6502Window = new Cpu6502Window();
	pCpu6502Window->SetCpu(pCpu);
	pWindowManager->AddWindow(pCpu6502Window, "Cpu6502");

	TiaWindow* pTiaWindow = new TiaWindow();
	pTiaWindow->SetTia(pTia);
	pWindowManager->AddWindow(pTiaWindow, "Tia");
	pStateSerialiser->AddStateSerialiser(pTiaWindow);

	RiotWindow* pRiotWindow = new RiotWindow();
	pRiotWindow->SetRiot(pRiot);
	pWindowManager->AddWindow(pRiotWindow, "Riot");
	pStateSerialiser->AddStateSerialiser(pRiotWindow);

	SystemWindow* pSystemWindow = new SystemWindow();
	pSystemWindow->SetSystem(pSystem);
	pWindowManager->AddWindow(pSystemWindow, "System");

	Perfect6502Window* pPerfect6502Window = new Perfect6502Window();
	pWindowManager->AddWindow(pPerfect6502Window, "Perfect6502");

	pStateSerialiser->DeserialiseAppConfig();

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

		pSystem->Update(dt);
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
	
	delete pMemory;

	LOGINFO("Exiting...\n");
	
	return 0;
}
