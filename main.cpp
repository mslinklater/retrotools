#include <ncurses.h>
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
		pMemory->Load(pConfig->GetLoadFilename(), loadAddress, &bytesLoaded);
	}

	pMemory->DumpToTTY(loadAddress, bytesLoaded);
	
	pDisassembler->Disassemble(loadAddress, bytesLoaded, loadAddress);
	pDisassembler->DumpToTTY();

	pMemory->Destroy();

	LOG("Exiting...\n");
	
	return 0;
}
