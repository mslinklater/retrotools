// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#pragma once
#include <string>
#include <set>

#include "commandcenter.h"
#include "cpu6502base.h"
#include "istateserialisation.h"

class Cpu6502Alpha : public Cpu6502Base, public ICommandHandler, public IStateSerialisation
{
	public:

		Cpu6502Alpha();
		virtual ~Cpu6502Alpha();
		
		// Register access
		
		uint16_t GetPC() override;
		uint8_t	GetAcc() override;
		uint8_t	GetX() override;
		uint8_t	GetY() override;
		uint8_t	GetStatus() override;
		uint8_t	GetSP() override;

		void SetPC(uint16_t pc) override;
		void SetAcc(uint8_t acc) override;
		void SetX(uint8_t x) override;
		void SetY(uint8_t y) override;
		void SetStatus(uint8_t status) override;
		void SetSP(uint8_t sp) override;

		ISERIALISATION_API

		// ITickable
		void CommitInputs() override {};			// commit state of input pins - so chip update order doesn't matter
		void Tick(bool clockState)override ;	// update the actual silicon state - based on the clockState
		// ~ITickable

		const Opcode* GetFetchOpcode() const override;
		const Opcode* GetExecuteOpcode() const override;

	private:
        
		void UpdateState(bool ignoreBreakpoints = false);		// kind of debug

		// ICommandProcessor
		bool HandleCommand(const std::shared_ptr<CommandBase> command) override;
		// ~ICommandProcessor
		
		bool		bHaltOnInstruction;
//		bool		bHalted;
		int32_t		ticksUntilExecution;
		uint16_t	next_pc;	// what the PC will be at next instruction
};
