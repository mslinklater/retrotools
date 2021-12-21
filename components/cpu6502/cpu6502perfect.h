// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "cpu6502base.h"

class Bus;

class Cpu6502Perfect : public Cpu6502Base
{
	public:
		Cpu6502Perfect();
		virtual ~Cpu6502Perfect();
		
	private:
		Bus*	pAddressBus;
		Bus*	pDataBus;
		Bus*	pSyncPin;
		Bus*	pVssPin;
		Bus*	pRdyPin;
		Bus*	pTimer0Pin;
		Bus*	pTimer1Pin;
		Bus*	pTimer2Pin;
		Bus*	pResPin;
		Bus*	pSoPin;
		Bus*	pIrqPin;
		Bus*	pNmiPin;
		Bus*	pVccPin;
		Bus*	pReadWritePin;
};
