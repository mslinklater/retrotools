#include "cpu6502_ldy.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"
#include "../utils/romfilebundle.h"

TEST_CASE("ldy", "[cpu6502]")
{
	// bury this
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool lda_loaded = bundle.Open("asm/unittests/ldy.prg");
	REQUIRE(lda_loaded);
	if(lda_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		// immediate
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero flag
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm_zero"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(numClocks == 2);

		// negative flag
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm_neg"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x80);
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero page
		pCpu->SetPC(bundle.GetSymbolValue("Start_zp"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x01);
		REQUIRE(numClocks == 3);

		// zero page, y forward
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpx1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x02);
		REQUIRE(numClocks == 6);

		// zero page, y back
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpx2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x00);
		REQUIRE(numClocks == 6);

		// absolute
		pCpu->SetPC(bundle.GetSymbolValue("Start_abs"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x01);
		REQUIRE(numClocks == 4);

		// absolute y no page
		pCpu->SetPC(bundle.GetSymbolValue("Start_absx1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x03);
		REQUIRE(numClocks == 6);

		// absolute y page
		pCpu->SetPC(bundle.GetSymbolValue("Start_absx2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x24);
		REQUIRE(numClocks == 7);
	}
}