#include "cpu6502_lda.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"
#include "../utils/romfilebundle.h"

TEST_CASE("lda", "[cpu6502]")
{
	// bury this
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool lda_loaded = bundle.Open("asm/unittests/lda.prg");
	REQUIRE(lda_loaded);
	if(lda_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		// immediate
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero
		pCpu->SetPC(bundle.GetSymbolValue("Start_zero"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero
		pCpu->SetPC(bundle.GetSymbolValue("Start_neg"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero page
		pCpu->SetPC(bundle.GetSymbolValue("Start_zp"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(numClocks == 3);

		// zero page,x negative
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpx1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(numClocks == 6);	// 2 for LDX, 4 for LDA ZP,X

		// zero page,x positive
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpx2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x02);
		REQUIRE(numClocks == 6);	// 2 for LDX, 4 for LDA ZP,X

		// abs
		pCpu->SetPC(bundle.GetSymbolValue("Start_abs"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(numClocks == 4);

		// abs, x 
		pCpu->SetPC(bundle.GetSymbolValue("Start_absx1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x02);
		REQUIRE(numClocks == 6);

		// abs, x page crossed
		pCpu->SetPC(bundle.GetSymbolValue("Start_absx2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x24);
		REQUIRE(numClocks == 7);

		// abs, y 
		pCpu->SetPC(bundle.GetSymbolValue("Start_absy1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x02);
		REQUIRE(numClocks == 6);

		// abs, y page crossed
		pCpu->SetPC(bundle.GetSymbolValue("Start_absy2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x24);
		REQUIRE(numClocks == 7);

		// indirect x
		pCpu->SetPC(bundle.GetSymbolValue("Start_indx"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x02);
		REQUIRE(numClocks == 8);

		// indirect y no page
		pCpu->SetPC(bundle.GetSymbolValue("Start_indy1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x04);
		REQUIRE(numClocks == 7);

		// indirect y page break
		pCpu->SetPC(bundle.GetSymbolValue("Start_indy2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x24);
		REQUIRE(numClocks == 8);
	}
}