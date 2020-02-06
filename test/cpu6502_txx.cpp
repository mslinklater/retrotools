#include "cpu6502_txx.h"

#include "../shared_cpp/catch.hpp"
#include "../components/cpu6502.h"
#include "../components/memory64k.h"
#include "../utils/romfilebundle.h"
#include "testcommon.h"

TEST_CASE("tax", "[cpu6502]")
{
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
		REQUIRE(numClocks == k6502TicksTAX);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tax"));
		pCpu->SetAcc(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTAX);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tax"));
		pCpu->SetAcc(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTAX);
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
		REQUIRE(numClocks == k6502TicksTAY);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tay"));
		pCpu->SetAcc(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTAY);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tay"));
		pCpu->SetAcc(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetY() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTAY);
	}
}

TEST_CASE("tsx", "[cpu6502]")
{
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

		pCpu->SetPC(bundle.GetSymbolValue("Start_tsx"));
		pCpu->SetSP(0x40);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTSX);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tsx"));
		pCpu->SetSP(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTSX);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tsx"));
		pCpu->SetSP(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetX() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTSX);
	}
}

TEST_CASE("txa", "[cpu6502]")
{
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

		pCpu->SetPC(bundle.GetSymbolValue("Start_txa"));
		pCpu->SetX(0x40);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);

		pCpu->SetPC(bundle.GetSymbolValue("Start_txa"));
		pCpu->SetX(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);

		pCpu->SetPC(bundle.GetSymbolValue("Start_txa"));
		pCpu->SetX(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);
	}
}

TEST_CASE("txs", "[cpu6502]")
{
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

		pCpu->SetPC(bundle.GetSymbolValue("Start_txs"));
		pCpu->SetX(0x40);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetSP() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);

		pCpu->SetPC(bundle.GetSymbolValue("Start_txs"));
		pCpu->SetX(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetSP() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);

		pCpu->SetPC(bundle.GetSymbolValue("Start_txs"));
		pCpu->SetX(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetSP() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);
	}
}

TEST_CASE("tya", "[cpu6502]")
{
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

		pCpu->SetPC(bundle.GetSymbolValue("Start_tya"));
		pCpu->SetY(0x40);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x40);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tya"));
		pCpu->SetY(0x00);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x00);
		REQUIRE(pCpu->GetZeroFlag());
		REQUIRE(!pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);

		pCpu->SetPC(bundle.GetSymbolValue("Start_tya"));
		pCpu->SetY(0x80);
		numClocks = pCpu->RunToBrk();
		REQUIRE(pCpu->GetAcc() == 0x80);
		REQUIRE(!pCpu->GetZeroFlag());
		REQUIRE(pCpu->GetNegativeFlag());
		REQUIRE(numClocks == k6502TicksTXA);
	}
}