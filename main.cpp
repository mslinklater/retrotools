//#include <stdio.h>
//#include <stdlib.h>
#include <ncurses.h>
#include "config.h"
#include "memory.h"
#include "cpu.h"
#include "vcs.h"
#include "disasm.h"

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

	// initialise a simple 6502 machine
	
	Memory* pMemory = new Memory();
	pMemory->Init();	
	
	vcs_Init();
    
	Cpu6502* pCpu = new Cpu6502();
    pCpu->Init(Cpu6502::k6507);
	pCpu->DumpInfo();

    memory_SetCPU(pCpu);
    
	
	disasm_Init();

	uint16_t bytesLoaded;
	uint16_t loadAddress = pConfig->GetLoadAddress();

	// need to grab the ROM filename from the config settings
	if(!pConfig->GetLoadFilename().empty())
	{
		memory_Load(pConfig->GetLoadFilename(), loadAddress, &bytesLoaded);
	}

	disasm_Disassemble(loadAddress, bytesLoaded, loadAddress);
	disasm_DumpToTTY();

	memory_Destroy();

	printf("Exiting...\n");
	
	return 0;
}
