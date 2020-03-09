// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <set>

#include "../shared_cpp/command.h"
#include "../config.h"
#include "cpu6502base.h"


//class IMemory;

class Cpu6502 : public Cpu6502Base, public ICommandProcessor, public IStateSerialisation
{
	public:

		Cpu6502();
		virtual ~Cpu6502();
		
		// Register access
		
		uint16_t GetPC();
		uint8_t	GetAcc();
		uint8_t	GetX();
		uint8_t	GetY();
		uint8_t	GetStatus();
		uint8_t	GetSP();

		void SetPC(uint16_t pc);
		void SetAcc(uint8_t acc);
		void SetX(uint8_t x);
		void SetY(uint8_t y);
		void SetStatus(uint8_t status);
		void SetSP(uint8_t sp);

		// IStateSerialisation
		void SerialiseState(json& object) override;
		void DeserialiseState(json& object) override;
		// ~IStateSerialisation

		// ITickable
		void CommitInputs(){};			// commit state of input pins - so chip update order doesn't matter
		void Tick(bool clockState);	// update the actual silicon state - based on the clockState
		// ~ITickable

		const Opcode* GetFetchOpcode() const;
		const Opcode* GetExecuteOpcode() const;

	private:
        
		void UpdateState(bool ignoreBreakpoints = false);		// kind of debug

		// ICommandProcessor
		bool HandleCommand(const Command &command) override;
		// ~ICommandProcessor
		
		bool		bHaltOnInstruction;
		bool		bHalted;
		int32_t		ticksUntilExecution;
		uint16_t	next_pc;	// what the PC will be at next instruction

};
