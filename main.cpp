//#include <stdio.h>
//#include <stdlib.h>
#include <ncurses.h>
#include "config.h"
#include "memory.h"
#include "cpu.h"
#include "vcs.h"
#include "disasm.h"
#include "symbolstore.h"

int main(int argc, char* argv[])
{
	printf("-- retrotool --\n");

	// check for command line args
	
	if(argc == 1)
	{
		printf("No command line args specified, exiting...\n");
		exit(0);
	}

	Config* pConfig = new Config();
	pConfig->Init();
	
	if(pConfig->ParseCommandLine(argc, argv) != kError_OK)
	{
		printf("Error parsing command line... aborting \n");
		exit(1);
	}

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

	uint16_t bytesLoaded;
	uint16_t loadAddress = pConfig->GetLoadAddress();

	// need to grab the ROM filename from the config settings
	if(!pConfig->GetLoadFilename().empty())
	{
		pMemory->Load(pConfig->GetLoadFilename(), loadAddress, &bytesLoaded);
	}

	pDisassembler->Disassemble(loadAddress, bytesLoaded, loadAddress);
	pDisassembler->DumpToTTY();

	pMemory->Destroy();

	printf("Exiting...\n");
	
	return 0;
}
