// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "cpu6502_clx.h"

#include "catch.hpp"
#include "../components/cpu6502/cpu6502alpha.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

#if RUN_TESTS

TEST_CASE("clx", "[cpu6502alpha]")
{
	std::unique_ptr<Cpu6502Alpha> pCpu(new Cpu6502Alpha());
	pCpu->Init(Cpu6502Base::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool clx_loaded = bundle.Open("asm/unittests/clx.prg");
	REQUIRE(clx_loaded);
	if(clx_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		pCpu->SetPC(bundle.GetSymbolValue("Start_clc"));
		pCpu->SetCarryFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(!pCpu->GetCarryFlag());
		REQUIRE(numClocks == k6502TicksCLC);

		pCpu->SetPC(bundle.GetSymbolValue("Start_cld"));
		pCpu->SetDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(!pCpu->GetDecimalFlag());
		REQUIRE(numClocks == k6502TicksCLD);

		pCpu->SetPC(bundle.GetSymbolValue("Start_cli"));
		pCpu->SetInterruptFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(!pCpu->GetInterruptFlag());
		REQUIRE(numClocks == k6502TicksCLI);

		pCpu->SetPC(bundle.GetSymbolValue("Start_clv"));
		pCpu->SetOverflowFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(numClocks == k6502TicksCLV);
	}
}

#endif // RUN_TESTS