#include "cpu6502test.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"
#include "../utils/romfilebundle.h"

TEST_CASE("lda", "[cpu6502]")
{
	// bury this
	Cpu6502* pCpu = new Cpu6502();
	Memory64K* pMemory = new Memory64K();
	pCpu->SetMemory(pMemory);

	RomFileBundle bundle;

	bundle.Open("lda.prg");

	// and this
//	uint16_t bytesRead;
//	MemoryUtils::LoadFileToMemory(pMemory, "asm/unittests/lda.prg", 0, &bytesRead);

	// set PC
	// run to BRK
	// check 

	REQUIRE(true);
}