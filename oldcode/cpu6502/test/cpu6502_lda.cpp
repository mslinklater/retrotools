// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "cpu6502_lda.h"

#include "catch.hpp"
#include "../components/cpu6502/cpu6502alpha.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

#if RUN_TESTS

TEST_CASE("lda", "[cpu6502alpha]")
{
	// bury this
	std::unique_ptr<Cpu6502Alpha> pCpu(new Cpu6502Alpha());
	pCpu->Init(Cpu6502Base::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint8_t numClocks;

	bool lda_loaded = bundle.Open("asm/unittests/lda.prg");
	REQUIRE(lda_loaded);
	if(lda_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		// immediate, no flags
		pCpu->SetPC(bundle.GetSymbolValue("Start_imm"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == bundle.GetSymbolValue("IMM_VALUE_NOFLAGS"));
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksLDAimm);

		// immediate, zero flag
		pCpu->SetPC(bundle.GetSymbolValue("Start_zero"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == bundle.GetSymbolValue("IMM_VALUE_ZEROFLAG"));
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksLDAimm);

		// immediate, neg flag
		pCpu->SetPC(bundle.GetSymbolValue("Start_neg"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == bundle.GetSymbolValue("IMM_VALUE_NEGFLAG"));
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksLDAimm);

		// zero page
		pCpu->SetPC(bundle.GetSymbolValue("Start_zp"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(numClocks == k6502TicksLDAzp);

		// zero page,x negative
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpx1"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(numClocks == k6502TicksLDXimm + k6502TicksLDAzpx);	// 2 for LDX, 4 for LDA ZP,X

		// zero page,x positive
		pCpu->SetPC(bundle.GetSymbolValue("Start_zpx2"));
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x02);
		REQUIRE(numClocks == k6502TicksLDXimm + k6502TicksLDAzpx);	// 2 for LDX, 4 for LDA ZP,X

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

#endif // RUN_TESTS