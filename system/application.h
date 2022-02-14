// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <memory>

#include <SDL.h>
#include <SDL_opengl.h>

class LuaVM;
class LogWindow;
class StateSerialiser;
class WindowManager;
class LuaWindow;
class HelpAboutWindow;
class HelpCommandsWindow;
class ResourcesWindow;
class MainWindow;
class MachineSimple6502;



class Application
{
public:
	static Application *Instance();

	void Init(int argc, char* argv[]);
	void UpdateLoop();
	int Close();

private:
	Application();
	~Application();

	void InitImGui();
	void ShutdownImGui();

	uint64_t	performanceFrequency;
	uint64_t	performanceCounterLast;
	SDL_GLContext gl_context;
	SDL_Window* window;
	int displayWidth;
	int displayHeight;

	std::shared_ptr<LuaVM> pLua;

	std::shared_ptr<LogWindow> pLogWindow;
	std::shared_ptr<StateSerialiser> pStateSerialiser;
	std::shared_ptr<WindowManager> pWindowManager;
	std::shared_ptr<LuaWindow> pLuaWindow;
	std::shared_ptr<HelpAboutWindow> pHelpAboutWindow;
	std::shared_ptr<HelpCommandsWindow> pHelpCommandsWindow;
	std::shared_ptr<ResourcesWindow> pResourcesWindow;
	std::shared_ptr<MainWindow> pMainWindow;

	std::shared_ptr<MachineSimple6502> pMachine;
};
