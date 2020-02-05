#include "cpu6502_txx.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"
#include "../utils/romfilebundle.h"

TEST_CASE("tax", "[cpu6502]")
{
	// bury this
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool txx_loaded = bundle.Open("asm/unittests/txx.prg");
	REQUIRE(txx_loaded);
	if(txx_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		pCpu->SetPC(bundle.GetSymbolValue("Start_tax"));
		pCpu->SetAcc(0x40);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tax"));
		pCpu->SetAcc(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tax"));
		pCpu->SetAcc(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);
	}
}

TEST_CASE("tay", "[cpu6502]")
{
	// bury this
	std::unique_ptr<Cpu6502> pCpu(new Cpu6502());
	pCpu->Init(Cpu6502::k6507);
	std::unique_ptr<Memory64K> pMemory(new Memory64K());
	pCpu->SetMemory(pMemory.get());

	RomFileBundle bundle;
	uint32_t numClocks;

	bool txx_loaded = bundle.Open("asm/unittests/txx.prg");
	REQUIRE(txx_loaded);
	if(txx_loaded)
	{
		bundle.CopyToMemory(pMemory.get());

		pCpu->SetPC(bundle.GetSymbolValue("Start_tay"));
		pCpu->SetAcc(0x40);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tay"));
		pCpu->SetAcc(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tay"));
		pCpu->SetAcc(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == 2);
	}
}