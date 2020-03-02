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
		
		uint16_t GetPC(){return reg.pc;}
		uint8_t	GetAcc(){return reg.acc;}
		uint8_t	GetX(){return reg.x;}
		uint8_t	GetY(){return reg.y;}
		uint8_t	GetStatus(){return reg.status;}
		uint8_t	GetSP(){return reg.sp;}

		void SetPC(uint16_t pc);
		void SetAcc(uint8_t acc){reg.acc = acc;}
		void SetX(uint8_t x){reg.x = x;}
		void SetY(uint8_t y){reg.y = y;}
		void SetStatus(uint8_t status){reg.status = status;}
		void SetSP(uint8_t sp){reg.sp = sp;}

		/// One single clock tick
//		void Tick(void);	// GET RID

		void ProcessInstruction(bool ignoreBreakpoints = false);		// kind of debug

		uint16_t	next_pc;	// what the PC will be at next instruction

		// IStateSerialisation
		void SerialiseState(json& object) override;
		void DeserialiseState(json& object) override;
		// ~IStateSerialisation

		uint32_t GetTicksUntilExecution(){return ticksUntilExecution;}

		// ITickable
		void CommitInputs(){};			// commit state of input pins - so chip update order doesn't matter
		void Tick(bool clockState);	// update the actual silicon state - based on the clockState
		// ~ITickable

		const Opcode* GetFetchOpcode() const;
		const Opcode* GetExecuteOpcode() const;

	private:
        
		// ICommandProcessor
		bool HandleCommand(const Command &command) override;
		// ~ICommandProcessor
		
		bool		bHaltOnInstruction;
		bool		bHalted;
		int32_t		ticksUntilExecution;

};
