// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "cpu6502_adc.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

#if RUN_TESTS

TEST_CASE("adc", "[cpu6502]")
{
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool adc_loaded = bundle.Open("asm/unittests/adc.prg");
	REQUIRE(adc_loaded);
	if(adc_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		//---------------------------------
		// NON BCD tests
		//---------------------------------

		// flags

		// 0 add immediate 1 no carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_imm_01"));
		pCpu->SetAcc(0x00);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCimm);

		// 0 add immediate 1 with carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_imm_01"));
		pCpu->SetAcc(0x00);
		pCpu->SetCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x02);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCimm);

		// 0 add immediate ff no carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_imm_ff"));
		pCpu->SetAcc(0x00);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0xff);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCimm);

		// 0 add immediate ff with carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_imm_ff"));
		pCpu->SetAcc(0x00);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCimm);

		// 0x80 add immediate 0x80 no carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_imm_80"));
		pCpu->SetAcc(0x80);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCimm);

		// 0x80 add immediate 0x80 with carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_imm_80"));
		pCpu->SetAcc(0x80);
		pCpu->SetCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCimm);


		// Addressing modes
		// 0x00 add zp 0x01 no carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_zp_1"));
		pCpu->SetAcc(0x00);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCzp);

		// 0x00 add zpx 0x01 no carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_zp_1x"));
		pCpu->SetAcc(0x00);
		pCpu->SetX(0x00);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCzpx);

		// 0x00 add abs 0x01 no carry
		pCpu->SetPC(bundle.GetSymbolValue("Start_adc_abs_1"));
		pCpu->SetAcc(0x00);
		pCpu->ClearCarryFlag();
		pCpu->ClearDecimalFlag();
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x01);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetOverflowFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksADCabs);
	}
}

#endif // RUN_TESTS