#include "SDL.h"
#include "config.h"
#include "memory.h"
#include "cpu.h"
#include "vcs.h"
#include "disasm.h"
#include "symbolstore.h"
#include "log.h"

int main(int argc, char* argv[])
{
	LOG("-- retrotool --\n");

	// check for command line args
	
	if(argc == 1)
	{
		LOG("No command line args specified, exiting...\n");
		exit(0);
	}

	Config* pConfig = new Config();
	pConfig->Init();
	
	if(pConfig->ParseCommandLine(argc, argv) != kError_OK)
	{
		LOG("Error parsing command line... aborting \n");
		exit(1);
	}

	// SDL stuff
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow(	"Distella", 
											SDL_WINDOWPOS_UNDEFINED, 
											SDL_WINDOWPOS_UNDEFINED, 
											640, 480, SDL_WINDOW_SHOWN);
	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
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

	SDL_Delay(3000);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	LOG("Exiting...\n");
	
	return 0;
}
