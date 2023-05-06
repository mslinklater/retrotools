// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "cpu6502_ldx.h"

#include "catch.hpp"
#include "../components/cpu6502/cpu6502alpha.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"
#include "../utils/romfilebundle.h"

#if RUN_TESTS

TEST_CASE("ldx", "[cpu6502alpha]")
{
	// bury this
	std::unique_ptr<Cpu6502Alpha> pCpu(new Cpu6502Alpha());
	pCpu->Init(Cpu6502Base::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool lda_loaded = bundle.Open("asm/unittests/ldx.prg");
	REQUIRE(lda_loaded);
	if(lda_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		// immediate
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero flag
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm_zero"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(numClocks == 2);

		// negative flag
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm_neg"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x80);
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		// zero page
		pCpu->SetPC(bundle.GetSymbolValue("Start_zp"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x01);
		REQUIRE(numClocks == 3);

		// zero page, y forward
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpy1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x02);
		REQUIRE(numClocks == 6);

		// zero page, y back
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpy2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x00);
		REQUIRE(numClocks == 6);

		// absolute
		pCpu->SetPC(bundle.GetSymbolValue("Start_abs"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x01);
		REQUIRE(numClocks == 4);

		// absolute y no page
		pCpu->SetPC(bundle.GetSymbolValue("Start_absy1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x03);
		REQUIRE(numClocks == 6);

		// absolute y page
		pCpu->SetPC(bundle.GetSymbolValue("Start_absy2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x24);
		REQUIRE(numClocks == 7);
	}
}

#endif // RUN_TESTS