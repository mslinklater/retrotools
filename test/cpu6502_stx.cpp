#include "cpu6502_stx.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

TEST_CASE("stx", "[cpu6502]")
{
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool stx_loaded = bundle.Open("asm/unittests/stx.prg");
	REQUIRE(stx_loaded);
	if(stx_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		// STA

		// Zero page
		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_zp"));
		pCpu->SetAcc(0xff);
		pMemory->Write(bundle.GetSymbolValue("ZP0"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("ZP0")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAzp);

		// Zero page,x
		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_zpx"));
		pCpu->SetAcc(0xff);
		pCpu->SetX(0xff);
		pMemory->Write(bundle.GetSymbolValue("ZPNEG"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("ZPNEG")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAzpx);

		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_zpx"));
		pCpu->SetAcc(0xfe);
		pCpu->SetX(0x01);
		pMemory->Write(bundle.GetSymbolValue("ZP1"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("ZP1")) == 0xfe);
		REQUIRE(numClocks == k6502TicksSTAzpx);

		// Absolute
		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_abs"));
		pCpu->SetAcc(0xff);
		pMemory->Write(bundle.GetSymbolValue("DATA0"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("DATA0")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAabs);

		// AbsoluteX
		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_absx"));
		pCpu->SetAcc(0xff);
		pCpu->SetX(0x01);
		pMemory->Write(bundle.GetSymbolValue("DATA1"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("DATA1")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAabsx);

		// AbsoluteY
		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_absy"));
		pCpu->SetAcc(0xff);
		pCpu->SetY(0x01);
		pMemory->Write(bundle.GetSymbolValue("DATA1"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("DATA1")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAabsy);

		// IndirectX
		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_indx"));
		pCpu->SetAcc(0xff);
		pCpu->SetX(0x00);
		pMemory->Write(bundle.GetSymbolValue("DATA1"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("DATA1")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAindx);

		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_indx"));
		pCpu->SetAcc(0xff);
		pCpu->SetX(0x02);
		pMemory->Write(bundle.GetSymbolValue("DATA2"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("DATA2")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAindx);

		pCpu->SetPC(bundle.GetSymbolValue("Start_sta_indx"));
		pCpu->SetAcc(0xff);
		pCpu->SetX(0xfe);
		pMemory->Write(bundle.GetSymbolValue("DATA0"), 0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pMemory->Read(bundle.GetSymbolValue("DATA0")) == 0xff);
		REQUIRE(numClocks == k6502TicksSTAindx);
	}
}
