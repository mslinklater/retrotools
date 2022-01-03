// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "cpu6502perfect.h"

Cpu6502Perfect::Cpu6502Perfect()
{
	pAddressBus = nullptr;
	pDataBus = nullptr;
	pSyncPin = nullptr;
	pVssPin = nullptr;
	pRdyPin = nullptr;
	pTimer0Pin = nullptr;
	pTimer1Pin = nullptr;
	pTimer2Pin = nullptr;
	pResPin = nullptr;
	pSoPin = nullptr;
	pIrqPin = nullptr;
	pNmiPin = nullptr;
	pVccPin = nullptr;
	pReadWritePin = nullptr;
}

Cpu6502Perfect::~Cpu6502Perfect()
{

}
