#include <SDL.h>
#include <SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl2.h"

#include "config.h"
#include "memory.h"
#include "cpu.h"
#include "vcs.h"
#include "disasm.h"
#include "symbolstore.h"
#include "log.h"

static Config* pConfig = 0;

void ProcessCommandLine(int argc, char* argv[])
{
	if(argc == 1)
	{
		LOG("No command line args specified, exiting...\n");
		exit(0);
	}	
	
	pConfig = new Config();
	pConfig->Init();
	
	if(pConfig->ParseCommandLine(argc, argv) != kError_OK)
	{
		LOG("Error parsing command line... aborting \n");
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	LOG("-- retrotool --\n");

	// check for command line args
	
	ProcessCommandLine(argc, argv);
	
	// SDL stuff
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}
	
	// Setup window
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);	
	SDL_Window* window = SDL_CreateWindow(	"Distella", 
											SDL_WINDOWPOS_CENTERED, 
											SDL_WINDOWPOS_CENTERED, 
											640, 480, windowFlags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();
	
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	bool done = false;
	bool show_demo_window = true;
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
		
		// start ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		
		// do stuff
		ImGui::ShowDemoWindow(&show_demo_window);
		
		// rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y,clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	// initialise components
	
	Memory* pMemory = new Memory();
	Cpu6502* pCpu = new Cpu6502();
	Disassembler* pDisassembler = new Disassembler();
	Vcs* pVcs = new Vcs();
	SymbolStore* pSymbolStore = new SymbolStore();
	
	pMemory->Init();	
	
	pVcs->Init();
    
    pCpu->Init(Cpu6502::k6507);
	pCpu->DumpInfo();
	pCpu->SetMemory(pMemory);

    pMemory->SetCPU(pCpu);
	
	pDisassembler->Init();
	pDisassembler->SetMemory(pMemory);
	pDisassembler->SetCpu(pCpu);

	uint16_t bytesLoaded;
	uint16_t loadAddress = pConfig->GetLoadAddress();

	// need to grab the ROM filename from the config settings
	if(!pConfig->GetLoadFilename().empty())
	{
		if (pMemory->Load(pConfig->GetLoadFilename(), loadAddress, &bytesLoaded) != kError_OK)
		{
			return -1;
		}
	}

	pMemory->DumpToTTY(loadAddress, bytesLoaded);
	
	pDisassembler->Disassemble(loadAddress, bytesLoaded, loadAddress);
	pDisassembler->DumpToTTY();

	pMemory->Destroy();

	LOG("Exiting...\n");
	
	return 0;
}
