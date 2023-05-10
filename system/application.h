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
class MachineBase;
class MessageCenter;

///
/// Application is the main application object. This is the first point of call for main() and should be used for all core application init, update & close logic.
///
class Application
{
public:
	/**
	 * @brief Get the Application singleton instance
	 * 
	 * @return Application* 
	 */
	static Application *Instance();

	/**
	 * @brief Application init
	 * 
	 * @param argc Passed in from main()
	 * @param argv Passed in from main()
	 */
	void Init(int argc, char* argv[]);

	/**
	 * @brief Handles the application update loop
	 * 
	 * You only call this once from main()
	 */
	void UpdateLoop();

	/**
	 * @brief Application clean-up on exit
	 * 
	 * @return int Non-zero indicates an error
	 */
	int Close();

	std::shared_ptr<LuaVM> GetLua(){ return lua; }

	void SetMachine(std::shared_ptr<MachineBase> pMachine);

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

	std::shared_ptr<LuaVM> lua;

	std::shared_ptr<MessageCenter> messageCenter;
	std::shared_ptr<LogWindow> logWindow;
	std::shared_ptr<StateSerialiser> stateSerialiser;
	std::shared_ptr<LuaWindow> luaWindow;
	std::shared_ptr<HelpAboutWindow> helpAboutWindow;
	std::shared_ptr<HelpCommandsWindow> helpCommandsWindow;
	std::shared_ptr<ResourcesWindow> resourcesWindow;
	std::shared_ptr<MainWindow> mainWindow;
	std::shared_ptr<WindowManager> windowManager;

	std::shared_ptr<MachineBase> pMachine;
};
