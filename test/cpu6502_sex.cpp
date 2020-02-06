#include "cpu6502_sex.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

TEST_CASE("sex", "[cpu6502]")
{
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool sex_loaded = bundle.Open("asm/unittests/sex.prg");
	REQUIRE(sex_loaded);
	if(sex_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		pCpu->SetPC(bundle.GetSymbolValue("Start_sec"));
		pCpu->ClearCarryFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetCarryFlag());
		REQUIRE(numClocks == k6502TicksSEC);

		pCpu->SetPC(bundle.GetSymbolValue("Start_sed"));
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetDecimalFlag());
		REQUIRE(numClocks == k6502TicksSED);

		pCpu->SetPC(bundle.GetSymbolValue("Start_sei"));
		pCpu->ClearInterruptFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetInterruptFlag());
		REQUIRE(numClocks == k6502TicksSEI);
	}
}
