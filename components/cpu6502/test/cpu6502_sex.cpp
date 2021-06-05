#include "cpu6502_sex.h"

#include "catch.hpp"
#include "../components/cpu6502/cpu6502alpha.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

#if RUN_TESTS

TEST_CASE("sex", "[cpu6502alpha]")
{
	std::unique_ptr<Cpu6502Alpha> pCpu(new Cpu6502Alpha());
	pCpu->Init(Cpu6502Base::k6507);
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

#endif // RUN_TESTS