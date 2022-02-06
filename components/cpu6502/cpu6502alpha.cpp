// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details


#include "system/common.h"
#include "cpu6502alpha.h"
#include "../interfaces/imemory.h"
#include "system/commands.h"

#define CPU_STATUS_MASK_CARRY		0x01
#define CPU_STATUS_MASK_ZERO		0x02
#define CPU_STATUS_MASK_INTERRPUT	0x04
#define CPU_STATUS_MASK_DECIMAL		0x08
#define CPU_STATUS_MASK_BREAK		0x10
#define CPU_STATUS_MASK_OVERFLOW	0x40
#define CPU_STATUS_MASK_NEGATIVE	0x80

// Constructor

Cpu6502Alpha::Cpu6502Alpha()
: bHaltOnInstruction(false)
, ticksUntilExecution(-1)
{
	LOGINFO("Cpu6502::Constructor");
//	CommandCenter::Instance()->Subscribe(Commands::kHaltCommand, this);
}

Cpu6502Alpha::~Cpu6502Alpha()
{
	LOGINFO("Cpu6502::Destructor");
//	CommandCenter::Instance()->Unsubscribe(Commands::kHaltCommand, this);
}

void Cpu6502Alpha::Tick(bool clockState)
{
	if(clockState)
	{
		// do work
		UpdateState();

		cyclesSinceBoot++;

		if(haltOnTick)
		{
//			Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU tick");
			haltOnTick = false;
		}
	}
}

const Cpu6502Base::Opcode* Cpu6502Alpha::GetFetchOpcode() const
{
	return GetExecuteOpcode();
}

const Cpu6502Base::Opcode* Cpu6502Alpha::GetExecuteOpcode() const
{
	uint8_t opcode = pMemory->Read(reg.pc);
	return &opcodes[opcode];
}

uint16_t Cpu6502Alpha::GetPC()
{
	return reg.pc;
}

uint8_t	Cpu6502Alpha::GetAcc()
{
	return reg.acc;
}

uint8_t	Cpu6502Alpha::GetX()
{
	return reg.x;
}

uint8_t	Cpu6502Alpha::GetY()
{
	return reg.y;
}

uint8_t	Cpu6502Alpha::GetStatus()
{
	return reg.status;
}

uint8_t	Cpu6502Alpha::GetSP()
{
	return reg.sp;
}

void Cpu6502Alpha::SetPC(uint16_t pc)
{
	reg.pc = pc; 
	next_pc = pc;
	ticksUntilExecution = -1;
}

void Cpu6502Alpha::SetAcc(uint8_t acc)
{
	reg.acc = acc;
}

void Cpu6502Alpha::SetX(uint8_t x)
{
	reg.x = x;
}

void Cpu6502Alpha::SetY(uint8_t y)
{
	reg.y = y;
}

void Cpu6502Alpha::SetStatus(uint8_t status)
{
	reg.status = status;
}

void Cpu6502Alpha::SetSP(uint8_t sp)
{
	reg.sp = sp;
}

void Cpu6502Alpha::UpdateState(bool ignoreBreakpoints)
{
	assert(pMemory != nullptr);

	ticksUntilExecution--;
	if((ticksUntilExecution == 1) && (bHaltOnInstruction))
	{
//		Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU instruction");
		bHaltOnInstruction = false;
	}
	if(ticksUntilExecution > 0)
	{
		return;
	}
	ticksUntilExecution = -1;

	// new PC comes in
	reg.pc = next_pc;

	// check for breakpoints
	if(!ignoreBreakpoints)
	{
		if(breakpoints.find(reg.pc) != breakpoints.end())
		{
//			Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU PC");
		}
	}

	// fetch next instruction...
	uint8_t opcode = pMemory->Read(reg.pc);
	Opcode* pOpcode = &opcodes[opcode];
	
	if(!pOpcode->valid)
	{
		LOGERRORF("Invalid opcode found 0x%02x", opcode);
		return;
	}

	uint16_t addr = 0;
	bool pageBoundaryCrossed = false;
	switch(pOpcode->addrMode)
	{
		case kAddrModeAccumulator:
			// operation on accumulator so no addr needs to be set.
			break;
		case kAddrModeImmediate:
			addr = reg.pc + 1;
			break;
		case kAddrModeZeroPage:
			addr = pMemory->Read(reg.pc+1);
			break;
		case kAddrModeZeroPageX:
			{
				addr = reg.x + pMemory->Read(reg.pc+1);
				addr &= 0x00ff;	// keep it in the zero page
			}
			break;
		case kAddrModeZeroPageY:
			{
				addr = reg.y + pMemory->Read(reg.pc+1);
				addr &= 0x00ff;	// keep it in the zero page
			}
			break;
		case kAddrModeAbsolute:
			addr = (pMemory->Read(reg.pc+2) << 8) | pMemory->Read(reg.pc+1);
			break;
		case kAddrModeAbsoluteX:
			{
				uint8_t msb = pMemory->Read(reg.pc+2);
				addr = (msb << 8) | pMemory->Read(reg.pc+1);
				addr += reg.x;
				pageBoundaryCrossed = (((addr >> 8) ^ msb) == 0) ? false : true;
			}
			break;
		case kAddrModeAbsoluteY:
			{
				uint8_t msb = pMemory->Read(reg.pc+2);
				addr = (msb << 8) | pMemory->Read(reg.pc+1);
				addr += reg.y;
				pageBoundaryCrossed = (((addr >> 8) ^ msb) == 0) ? false : true;
			}
			break;
		case kAddrModeIndirect:
			{
				addr = (uint16_t)(pMemory->Read(reg.pc+1)) | (uint16_t)((pMemory->Read(reg.pc+2) << 8));
			}
			break;
		case kAddrModeIndirectY:
			{
				uint16_t loc = pMemory->Read(reg.pc+1);
				uint8_t msb = pMemory->Read(loc+1);
				uint16_t tempaddr = pMemory->Read(loc) | (msb << 8);
				addr += tempaddr + reg.y;
				pageBoundaryCrossed = (((addr >> 8) ^ msb) == 0) ? false : true;
			}
			break;
		case kAddrModeIndirectX:
			{
				uint16_t loc = (pMemory->Read(reg.pc+1) + reg.x) & 0xff;
				addr = (pMemory->Read((loc+1)&0xff) << 8) | pMemory->Read(loc);
			}
			break;
		case kAddrModeRelative:
			addr = reg.pc + (int8_t)pMemory->Read(reg.pc+1);
			break;
		case kAddrModeImplied:
			break;
		default:
			LOGERRORF("Unemulated addressing mode %s", addrModeStrings[pOpcode->addrMode].c_str());
//			Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU emu problem - see log");
			return;
			break;
	}

	pMemory->SetHasBeenExecuted(reg.pc, pOpcode->length);

	// need flags stuff here too
	switch(pOpcode->mnemonic)
	{
		case kMnemonic_ADC:
			{
				if(GetDecimalFlag())
				{
					uint8_t val = pMemory->Read(addr);
					uint8_t valLo = val & 0x0f;
					uint8_t valHi = (val & 0xf0) >> 4;
					uint8_t accLo = reg.acc & 0x0f;
					uint8_t accHi = (reg.acc & 0xf0) >> 4;

					uint8_t resLo = valLo + accLo + (GetCarryFlag() ? 1 : 0);
					bool loCarry = resLo >= 10;
					if(resLo > 10) resLo -= 10;
					uint8_t resHi = valHi + accHi + (loCarry ? 1 : 0);
					if(resHi > 10)
					{
						SetCarryFlag();
					}
					else
					{
						ClearCarryFlag();
					}
					if(resHi > 10) resHi -= 10;
					reg.acc = (resHi << 4) | resLo;
				}
				else
				{
					uint16_t val = (uint16_t)reg.acc;
					uint16_t addValue = pMemory->Read(addr) + (GetCarryFlag() ? 1 : 0);
					val += addValue;
					reg.acc = val & 0x00ff;
					(val & 0x0100) ? SetCarryFlag() : ClearCarryFlag();
					(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
					(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
					// overflow flag
					int8_t vals = (int8_t)reg.acc;
					int16_t val16s = (int16_t)vals;
					int8_t adds = ((int8_t)pMemory->Read(addr)) + (GetCarryFlag() ? 1 : 0);
					int16_t add16s = (int16_t)adds;
					int16_t result = val16s + add16s;
					(result <= 127 && result >= -128) ? ClearOverflowFlag() : SetOverflowFlag();
				}
				switch(pOpcode->addrMode)
				{
					case kAddrModeImmediate: ticksUntilExecution = k6502TicksADCimm; break;
					case kAddrModeZeroPage: ticksUntilExecution = k6502TicksADCzp; break;
					case kAddrModeZeroPageX: ticksUntilExecution = k6502TicksADCzpx; break;
					case kAddrModeAbsolute: ticksUntilExecution = k6502TicksADCabs; break;
					case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? k6502TicksADCabsx+1 : k6502TicksADCabsx; break;
					case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? k6502TicksADCabsy+1 : k6502TicksADCabsy; break;
					case kAddrModeIndirectX: ticksUntilExecution = k6502TicksADCindx; break;
					case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? k6502TicksADCindy+1 : k6502TicksADCindy; break;
					default: break;
				}
				next_pc = reg.pc + pOpcode->length;				
			}
			break;
		case kMnemonic_AND:
			{
				uint8_t val = pMemory->Read(addr);
				reg.acc &= val;
				(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
				(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = k6502TicksANDimm; break;
				case kAddrModeZeroPage: ticksUntilExecution = k6502TicksANDzp; break;
				case kAddrModeZeroPageX: ticksUntilExecution = k6502TicksANDzpx; break;
				case kAddrModeAbsolute: ticksUntilExecution = k6502TicksANDabs; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? k6502TicksANDabsx+1 : k6502TicksANDabsx; break;
				case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? k6502TicksANDabsy+1 : k6502TicksANDabsy; break;
				case kAddrModeIndirectX: ticksUntilExecution = k6502TicksANDindx; break;
				case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? k6502TicksANDindy+1 : k6502TicksANDindy; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;				
			break;
		case kMnemonic_ASL:
			if(pOpcode->addrMode == kAddrModeAccumulator)
			{
				(reg.acc & 0x80) ? SetCarryFlag() : ClearCarryFlag();
				reg.acc <<= 1;
				(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
				(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			}
			else
			{
				uint8_t val = pMemory->Read(addr);
				(val & 0x80) ? SetCarryFlag() : ClearCarryFlag();
				val <<= 1;
				(val == 0) ? SetZeroFlag() : ClearZeroFlag();
				(val & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
				pMemory->Write(addr, val);
			}			
			switch(pOpcode->addrMode)
			{
				case kAddrModeAccumulator: ticksUntilExecution = k6502TicksASLacc; break;
				case kAddrModeZeroPage: ticksUntilExecution = k6502TicksASLzp; break;
				case kAddrModeZeroPageX: ticksUntilExecution = k6502TicksASLzpx; break;
				case kAddrModeAbsolute: ticksUntilExecution = k6502TicksASLabs; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = k6502TicksASLabsx; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;				
			break;
		case kMnemonic_BIT:
			{
				uint8_t val = pMemory->Read(addr);
				val & 0x80 ? SetNegativeFlag() : ClearNegativeFlag();
				val & 0x40 ? SetOverflowFlag() : ClearOverflowFlag();
				uint8_t anded = reg.acc & val;
				(anded == 0) ? SetZeroFlag() : ClearZeroFlag();
				switch(pOpcode->addrMode)
				{
					case kAddrModeZeroPage: ticksUntilExecution = k6502TicksBITzp; break;
					case kAddrModeAbsolute: ticksUntilExecution = k6502TicksBITabs; break;
					default: break;
				}
			}
			next_pc = reg.pc + pOpcode->length;				
			break;
		case kMnemonic_BCC:
			if(!GetCarryFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? k6502TicksBranchTakenPage : k6502TicksBranchTaken;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				ticksUntilExecution = k6502TicksBranchNotTaken;
				next_pc = reg.pc + pOpcode->length;				
			}
			break;
		case kMnemonic_BCS:
			if(GetCarryFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				next_pc = reg.pc + pOpcode->length;				
				ticksUntilExecution = 2;
			}
			break;
		case kMnemonic_BEQ:
			if(GetZeroFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				next_pc = reg.pc + pOpcode->length;				
				ticksUntilExecution = 2;
			}
			break;
		case kMnemonic_BPL:
			if(!GetNegativeFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				next_pc = reg.pc + pOpcode->length;				
				ticksUntilExecution = 2;
			}
			break;
		case kMnemonic_BMI:
			if(GetNegativeFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				next_pc = reg.pc + pOpcode->length;				
				ticksUntilExecution = 2;
			}
			break;
		case kMnemonic_BNE:
			if(!GetZeroFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				ticksUntilExecution = 2;
				next_pc = reg.pc + pOpcode->length;				
			}
			break;
		case kMnemonic_BRK:
			ticksUntilExecution = 2;
//			Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU BRK instruction");
			break;
		case kMnemonic_BVC:
			if(!GetOverflowFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				next_pc = reg.pc + pOpcode->length;				
				ticksUntilExecution = 2;
			}
			break;
		case kMnemonic_BVS:
			if(GetOverflowFlag())
			{
				ticksUntilExecution = ((reg.pc & 0xff00) ^ (addr & 0xff00)) ? 4 : 3;
				next_pc = addr + pOpcode->length;				
			}
			else
			{
				next_pc = reg.pc + pOpcode->length;				
				ticksUntilExecution = 2;
			}
			break;
		case kMnemonic_CLC:
			ClearCarryFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksCLC;
			break;
		case kMnemonic_CLD:
			ClearDecimalFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksCLD;
			break;
		case kMnemonic_CLI:
			ClearInterruptFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksCLI;
			break;
		case kMnemonic_CLV:
			ClearOverflowFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksCLV;
			break;
		case kMnemonic_CMP:
			{
				uint8_t val = pMemory->Read(addr);
				int8_t result = reg.acc - val;
				(reg.acc == val) ? SetZeroFlag() : ClearZeroFlag();
				(reg.acc >= val) ? SetCarryFlag() : ClearCarryFlag();
				(result & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				case kAddrModeIndirectX: ticksUntilExecution = 6; break;
				case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? 6 : 5; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_CPX:
			{
				uint8_t val = pMemory->Read(addr);
				(reg.x == val) ? SetZeroFlag() : ClearZeroFlag();
				(reg.x >= val) ? SetCarryFlag() : ClearCarryFlag();
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_CPY:
			{
				uint8_t val = pMemory->Read(addr);
				(reg.y == val) ? SetZeroFlag() : ClearZeroFlag();
				(reg.y >= val) ? SetCarryFlag() : ClearCarryFlag();
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_DEC: // complete
			{
				uint8_t value = pMemory->Read(addr);
				value--;
				pMemory->Write(addr, value);
				(value == 0) ? SetZeroFlag() : ClearZeroFlag();
				(value & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeZeroPage: ticksUntilExecution = 5; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 6; break;
				case kAddrModeAbsolute: ticksUntilExecution = 6; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = 7; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_DEX:	// complete
			reg.x--;
			(reg.x == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.x & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_DEY:	// complete
			reg.y--;
			(reg.y == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.y & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_EOR:	// complete
			{
				uint8_t val = pMemory->Read(addr);
				reg.acc ^= val;
				(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
				(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
				next_pc = reg.pc + pOpcode->length;
				switch(pOpcode->addrMode)
				{
					case kAddrModeImmediate: ticksUntilExecution = 2; break;
					case kAddrModeZeroPage: ticksUntilExecution = 3; break;
					case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
					case kAddrModeAbsolute: ticksUntilExecution = 4; break;
					case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
					case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
					case kAddrModeIndirectX: ticksUntilExecution = 6; break;
					case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? 6 : 5; break;
					default: break;
				}
			}
			break;
		case kMnemonic_INC:	// complete
			{
				uint8_t val = pMemory->Read(addr);
				val++;
				(val == 0) ? SetZeroFlag() : ClearZeroFlag();
				(val & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();				
				pMemory->Write(addr, val);
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeZeroPage: ticksUntilExecution = 5; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 6; break;
				case kAddrModeAbsolute: ticksUntilExecution = 6; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = 7; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_INX:	// complete
			reg.x++;
			(reg.x == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.x & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_INY:	// complete
			reg.y++;
			(reg.y == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.y & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_JMP:
			switch(pOpcode->addrMode)
			{
				case kAddrModeAbsolute: ticksUntilExecution = 3; break;
				case kAddrModeIndirect: ticksUntilExecution = 5; break;
				default: break;
			}
			next_pc = addr;
			break;
		case kMnemonic_JSR:
			{
				uint16_t returnAddress = reg.pc + 2;	// next instruction - 1
				pMemory->Write(reg.sp--, (uint8_t)(returnAddress & 0x00ff));
				pMemory->Write(reg.sp--, (uint8_t)((returnAddress & 0xff00) >> 8));
				next_pc = addr;
				ticksUntilExecution = 6;
			}
			break;
		case kMnemonic_LDA:
			reg.acc = pMemory->Read(addr);
			(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = k6502TicksLDAimm; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				case kAddrModeIndirectX: ticksUntilExecution = 6; break;
				case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? 6 : 5; break;
				default: break;
			}
			break;
		case kMnemonic_LDX:
			reg.x = pMemory->Read(addr);
			(reg.x == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.x & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageY: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				default: break;
			}
			break;
		case kMnemonic_LDY:
			reg.y = pMemory->Read(addr);
			(reg.y == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.y & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				default: break;
			}
			break;
		case kMnemonic_LSR:
			if(pOpcode->addrMode == kAddrModeAccumulator)
			{
				(reg.acc & 0x01) ? SetCarryFlag() : ClearCarryFlag();
				reg.acc >>= 1;
				reg.acc &= 0x7f;
				(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
				(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			}
			else
			{
				uint8_t val = pMemory->Read(addr);
				(val & 0x01) ? SetCarryFlag() : ClearCarryFlag();
				val >>= 1;
				val &= 0x7f;
				(val == 0) ? SetZeroFlag() : ClearZeroFlag();
				(val & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
				pMemory->Write(addr, val);
			}
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeAccumulator: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 5; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 6; break;
				case kAddrModeAbsolute: ticksUntilExecution = 6; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = 7; break;
				default: break;
			}
			break;
		case kMnemonic_NOP:
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeImplied: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				default: break;
			}
			break;
		case kMnemonic_ORA:
			{
				uint8_t val = pMemory->Read(addr);
				reg.acc |= val;
				(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();				
				(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();				
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeImmediate: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
				case kAddrModeIndirectX: ticksUntilExecution = 6; break;
				case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? 6 : 5; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_PHA: 
			addr = 0x0100 | reg.sp--;
			pMemory->Write(addr, reg.acc);
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 3;
			break;
		case kMnemonic_PLA: 
			addr = 0x0100 | ++reg.sp;
			reg.acc = pMemory->Read(addr);
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 4;
			break;
		case kMnemonic_PHP: 
			addr = 0x0100 | reg.sp--;
			pMemory->Write(addr, reg.status);
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 3;
			break;
		case kMnemonic_PLP: 
			addr = 0x0100 | ++reg.sp;
			reg.status = pMemory->Read(addr);
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 4;
			break;
		case kMnemonic_ROL: 
			{
				bool carry = GetCarryFlag();
				if(pOpcode->addrMode == kAddrModeAccumulator)
				{
					(reg.acc & 0x80) ? SetCarryFlag() : ClearCarryFlag();
					reg.acc <<= 1;
					if(carry)
					{
						reg.acc |= 0x01;
					}
				}
				else
				{
					uint8_t val = pMemory->Read(addr);
					(val & 0x80) ? SetCarryFlag() : ClearCarryFlag();
					val <<= 1;
					if(carry)
					{
						val |= 0x01;
					}
					pMemory->Write(addr, val);
				}
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeAccumulator: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 5; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 6; break;
				case kAddrModeAbsolute: ticksUntilExecution = 6; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = 7; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_ROR: 
			{
				bool carry = GetCarryFlag();
				if(pOpcode->addrMode == kAddrModeAccumulator)
				{
					(reg.acc & 0x01) ? SetCarryFlag() : ClearCarryFlag();
					reg.acc >>= 1;
					if(carry)
					{
						reg.acc |= 0x80;
					}
				}
				else
				{
					uint8_t val = pMemory->Read(addr);
					(val & 0x01) ? SetCarryFlag() : ClearCarryFlag();
					val >>= 1;
					if(carry)
					{
						val |= 0x80;
					}
					pMemory->Write(addr, val);
				}
			}
			switch(pOpcode->addrMode)
			{
				case kAddrModeAccumulator: ticksUntilExecution = 2; break;
				case kAddrModeZeroPage: ticksUntilExecution = 5; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 6; break;
				case kAddrModeAbsolute: ticksUntilExecution = 6; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = 7; break;
				default: break;
			}
			next_pc = reg.pc + pOpcode->length;
			break;
		case kMnemonic_RTS:
			next_pc = ((uint16_t)pMemory->Read(++reg.sp)) << 8;
			next_pc |= (uint16_t)pMemory->Read(++reg.sp);
			next_pc += pOpcode->length;
			ticksUntilExecution = 6;
			break;
		case kMnemonic_SBC:
			{
				if(GetDecimalFlag())
				{
					uint8_t val = pMemory->Read(addr);
					uint8_t valLo = val & 0x0f;
					uint8_t valHi = (val & 0xf0) >> 4;
					uint8_t accLo = reg.acc & 0x0f;
					uint8_t accHi = (reg.acc & 0xf0) >> 4;

					uint8_t resLo = valLo + accLo + (GetCarryFlag() ? 1 : 0);
					bool loCarry = resLo >= 10;
					if(resLo > 10) resLo -= 10;
					uint8_t resHi = valHi + accHi + (loCarry ? 1 : 0);
					if(resHi > 10)
					{
						SetCarryFlag();
					}
					else
					{
						ClearCarryFlag();
					}
					if(resHi > 10) resHi -= 10;
					reg.acc = (resHi << 4) | resLo;
				}
				else
				{
					uint16_t val = (uint16_t)reg.acc;
					uint16_t subValue = pMemory->Read(addr) - (GetCarryFlag() ? 0 : 1);
					val -= subValue;
					reg.acc = val & 0x00ff;
					(val & 0x0100) ? SetCarryFlag() : ClearCarryFlag();
					(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
					(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
					// overflow flag
					int8_t vals = (int8_t)reg.acc;
					int16_t val16s = (int16_t)vals;
					int8_t subs = ((int8_t)pMemory->Read(addr)) - (GetCarryFlag() ? 0 : 1);
					int16_t sub16s = (int16_t)subs;
					int16_t result = val16s - sub16s;
					(result <= 127 && result >= -128) ? ClearOverflowFlag() : SetOverflowFlag();
				}
				switch(pOpcode->addrMode)
				{
					case kAddrModeImmediate: ticksUntilExecution = 2; break;
					case kAddrModeZeroPage: ticksUntilExecution = 3; break;
					case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
					case kAddrModeAbsolute: ticksUntilExecution = 4; break;
					case kAddrModeAbsoluteX: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
					case kAddrModeAbsoluteY: ticksUntilExecution = pageBoundaryCrossed ? 5 : 4; break;
					case kAddrModeIndirectX: ticksUntilExecution = 6; break;
					case kAddrModeIndirectY: ticksUntilExecution = pageBoundaryCrossed ? 6 : 5; break;
					default: break;
				}
				next_pc = reg.pc + pOpcode->length;				
			}
			break;
		case kMnemonic_SEC:
			SetCarryFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_SED:
			SetDecimalFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_SEI:
			SetInterruptFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = 2;
			break;
		case kMnemonic_STA:
			pMemory->Write(addr, reg.acc);
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeZeroPage: ticksUntilExecution = k6502TicksSTAzp; break;
				case kAddrModeZeroPageX: ticksUntilExecution = k6502TicksSTAzpx; break;
				case kAddrModeAbsolute: ticksUntilExecution = k6502TicksSTAabs; break;
				case kAddrModeAbsoluteX: ticksUntilExecution = k6502TicksSTAabsx; break;
				case kAddrModeAbsoluteY: ticksUntilExecution = k6502TicksSTAabsy; break;
				case kAddrModeIndirectX: ticksUntilExecution = k6502TicksSTAindx; break;
				case kAddrModeIndirectY: ticksUntilExecution = k6502TicksSTAindy; break;
				default: break;
			}
			break;
		case kMnemonic_STX:
			pMemory->Write(addr, reg.x);
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageY: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				default: break;
			}
			break;
		case kMnemonic_STY:
			pMemory->Write(addr, reg.y);
			next_pc = reg.pc + pOpcode->length;
			switch(pOpcode->addrMode)
			{
				case kAddrModeZeroPage: ticksUntilExecution = 3; break;
				case kAddrModeZeroPageX: ticksUntilExecution = 4; break;
				case kAddrModeAbsolute: ticksUntilExecution = 4; break;
				default: break;
			}
			break;
		case kMnemonic_TAX:
			reg.x = reg.acc;
			(reg.x == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.x & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksTAX;
			break;
		case kMnemonic_TAY:
			reg.y = reg.acc;
			(reg.y == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.y & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksTAY;
			break;
		case kMnemonic_TSX:
			reg.x = reg.sp;
			next_pc = reg.pc + pOpcode->length;
			(reg.x == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.x & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			ticksUntilExecution = k6502TicksTSX;
			break;
		case kMnemonic_TXA:
			reg.acc = reg.x;
			(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksTXA;
			break;
		case kMnemonic_TXS:
			reg.sp = reg.x;
			next_pc = reg.pc + pOpcode->length;
			(reg.sp == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.sp & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			ticksUntilExecution = k6502TicksTXS;
			break;
		case kMnemonic_TYA:
			reg.acc = reg.y;
			(reg.acc == 0) ? SetZeroFlag() : ClearZeroFlag();
			(reg.acc & 0x80) ? SetNegativeFlag() : ClearNegativeFlag();
			next_pc = reg.pc + pOpcode->length;
			ticksUntilExecution = k6502TicksTYA;
			break;
		default:
			LOGERRORF("Unemulated mnemonic %s", mnemonicStrings[pOpcode->mnemonic].c_str());
			//Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU emu problem - see log");
			break;
	}
	if(ticksUntilExecution == -1)
	{
		LOGERRORF("Untimed mnemonic %s", mnemonicStrings[pOpcode->mnemonic].c_str());
		//Commands::Halt(true, Commands::HaltCommand::kHalt, "CPU emu problem - see log");
	}
}

void Cpu6502Alpha::SerialiseState(json& object)
{
	LOGINFO("Cpu6502::SerialiseState");
	json cpuJson = json::object();

	json breakpointsJson = json::array();

	for(auto breakpoint : breakpoints)
	{
		json breakpointJson = json::object();
		breakpointJson["addr"] = breakpoint;
		breakpointsJson.push_back(breakpointJson);
	}

	cpuJson["breakpoints"] = breakpointsJson;
	object["cpu"] = cpuJson;
}

void Cpu6502Alpha::DeserialiseState(json& object)
{
	LOGINFO("Cpu6502::DeserialiseState");
	json cpuJson = object["cpu"];
	if(cpuJson.is_object())
	{
		json breakpointsArrayJson = cpuJson["breakpoints"];
		if(breakpointsArrayJson.is_array())
		{
			for(auto breakpointJson : breakpointsArrayJson)
			{
				SetBreakpoint(breakpointJson["addr"].get<uint16_t>());
			}
		}
	}
}

bool Cpu6502Alpha::HandleCommand(const std::shared_ptr<CommandBase> command)
{
#if 0
	if(command.name == Commands::kHaltCommand)
	{
		if((command.payload == "false") && (command.payload2 == Commands::kHaltCommandTickCpu))
		{
			haltOnTick = true;
		}
		if((command.payload == "false") && (command.payload2 == Commands::kHaltCommandCpuInstruction))
		{
			bHaltOnInstruction = true;
		}

		if(command.payload == "true")
		{
			bHalted = true;
		}
		else
		{
			bHalted = false;	
		}
		
	}
#endif
	return false;
}
