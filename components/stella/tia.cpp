// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <assert.h>

#include "common.h"
#include "tia.h"
#include "../commands.h"
#include "command.h"

#define HALT_ON_UNIMPLEMENTED 0

Tia::Tia()
: rasterX(0)
, rasterY(0)
, frameNum(0)
, rasterCount(0)
, rasterCountLatched(0)
, region(ERegion::NTSC)
, bHaltOnTick(false)
, bHaltOnHBlank(false)
, bHaltOnVBlank(false)
, ticksSinceBoot(0)
{
	CommandCenter::Instance()->Subscribe(Commands::kHaltCommand, this);
	for(int i=0 ; i<kNumReadRegisters ; i++)
	{
		readRegisters[i] = 0;
		bReadBreakpoints[i] = false;
	}
	for(int i=0 ; i<kNumWriteRegisters ; i++)
	{
		writeRegisters[i] = 0;
		bWriteBreakpoints[i] = false;
	}

	for(int y=0 ; y<kOutputVerticalResolution ; y++)
	{
		for(int x=0 ; x<kOutputHorizontalResolution ; x++)
		{
			pixels[(y*kOutputHorizontalResolution) + x] = y & 7;
		}
	}

	// Playfield bits
	for(int i=0 ; i<kNumPlayfieldBits ; i++)
	{
		playfieldBits[i] = false;
	}

	// sprite bits
	for(int i=0 ; i<kNumSpriteBits ; i++)
	{
		sprite0Bits[i] = false;
		sprite1Bits[i] = false;
	}

	bShowPF = true;
	bShowP0 = true;
	bShowP1 = true;
}

Tia::~Tia()
{
}

void Tia::Tick()
{
	// do tick

	// Begin actual render logic
	pixels[rasterX + (rasterY*228)] = GetCOLUBK();
	if(rasterX >= 68)
	{
		int playfieldIndex = (rasterX-68)/4;
		if(bShowPF && playfieldBits[playfieldIndex])
		{
			pixels[rasterX + (rasterY*228)] = GetCOLUPF();
		}
		if(bShowP0 && sprite0Bits[rasterX-68])
		{
			pixels[rasterX + (rasterY*228)] = GetCOLUP0();
		}
		if(bShowP1 && sprite1Bits[rasterX-68])
		{
			pixels[rasterX + (rasterY*228)] = GetCOLUP1();
		}
	}
	// End actual render logic
	cpuActive[rasterX + (rasterY*228)] = !bCpuWaitingForHsync;

	// now move
	rasterX++;
	if(rasterX >= 228)
	{
		// hblank
		vblankActive[rasterY] = GetVBLANK() != 0;
		rasterX = 0;
		rasterY++;
		
		// stop overruns
		if(rasterY >= kOutputVerticalResolution)
		{
			rasterY = kOutputVerticalResolution;
		}

		rasterCount++;
		if(bHaltOnHBlank)
		{
			Commands::Halt(true, Commands::HaltCommand::kHalt, "TIA HBlank");
			bHaltOnHBlank = false;
		}
		bCpuWaitingForHsync = false;
		// end hblank
	}

	ticksSinceBoot++;

	if(bHaltOnTick)
	{
		Commands::Halt(true, Commands::HaltCommand::kHalt, "TIA Tick");
		bHaltOnTick = false;
	}
}

const uint8_t* Tia::GetPalette()
{
	switch(region)
	{
		case NTSC:
			return &paletteNTSC[0];
		case PAL:
			return &palettePAL[0];
		default:	// SECAM
			return &paletteSECAM[0];
	}
}

void Tia::RebuildPlayfieldBits()
{
	uint8_t PF0 = GetPF0();
	uint8_t PF1 = GetPF1();
	uint8_t PF2 = GetPF2();

	playfieldBits[0] = PF0 & 0x10;
	playfieldBits[1] = PF0 & 0x20;
	playfieldBits[2] = PF0 & 0x40;
	playfieldBits[3] = PF0 & 0x80;
	playfieldBits[4] = PF1 & 0x80;
	playfieldBits[5] = PF1 & 0x40;
	playfieldBits[6] = PF1 & 0x20;
	playfieldBits[7] = PF1 & 0x10;
	playfieldBits[8] = PF1 & 0x08;
	playfieldBits[9] = PF1 & 0x04;
	playfieldBits[10] = PF1 & 0x02;
	playfieldBits[11] = PF1 & 0x01;
	playfieldBits[12] = PF2 & 0x01;
	playfieldBits[13] = PF2 & 0x02;
	playfieldBits[14] = PF2 & 0x04;
	playfieldBits[15] = PF2 & 0x08;
	playfieldBits[16] = PF2 & 0x10;
	playfieldBits[17] = PF2 & 0x20;
	playfieldBits[18] = PF2 & 0x40;
	playfieldBits[19] = PF2 & 0x80;
	if(writeRegisters[kCTRLPF] & 0x01)
	{
		// mirror
		for(int i=0 ; i<20 ; i++)
		{
			playfieldBits[39-i] = playfieldBits[i];
		}	
	}
	else
	{
		// duplicate
		for(int i=0 ; i<20 ; i++)
		{
			playfieldBits[i+20] = playfieldBits[i];
		}	
	}
}

void Tia::RebuildSprite0Bits()
{
	uint8_t numCopies = 0;
	uint8_t scale = 0;
	uint8_t gap = 0;
	uint8_t nusiz = GetNUSIZ0() & 0x07;

	switch(nusiz)
	{
		case 0: numCopies=1; scale=1; break;
		case 1: numCopies=2; scale=1; gap=1; break;
		case 2: numCopies=2; scale=1; gap=3; break;
		case 3: numCopies=3; scale=1; gap=1; break;
		case 4: numCopies=2; scale=1; gap=7; break;
		case 5: numCopies=1; scale=2; break;
		case 6: numCopies=3; scale=1; gap=3; break;
		case 7: numCopies=1; scale=4; break;
	}

	// clear
	for(int i=0 ; i<kHBlankClocks ; i++)
	{
		sprite0Bits[i] = false;
	}
	uint8_t val = GetGRP0();

	int offset = 0;

	for(int iCopy = 0 ; iCopy<numCopies ; iCopy++)
	{
		// sprite
		for(int bit=7 ; bit >= 0 ; bit--)
		{
			for(int rep=0 ; rep<scale ; rep++)
			{
				int i = resP0Pos+offset;
				if(i < kNumSpriteBits)
				{
					sprite0Bits[i] = val & (0x01 << bit);
				}
				offset++;
			}
		}
		// gap
		for(int iGap=0 ; iGap<gap ; iGap++)
		{
			for(int bit=7 ; bit >= 0 ; bit--)
			{
				int i = resP0Pos+offset;
				if(i < kNumSpriteBits)
				{
					sprite0Bits[i] = false;
				}
				offset++;
			}
		}
	}
}

void Tia::RebuildSprite1Bits()
{
	uint8_t numCopies = 0;
	uint8_t scale = 0;
	uint8_t gap = 0;
	uint8_t nusiz = GetNUSIZ1() & 0x07;

	switch(nusiz)
	{
		case 0: numCopies=1; scale=1; break;
		case 1: numCopies=2; scale=1; gap=1; break;
		case 2: numCopies=2; scale=1; gap=3; break;
		case 3: numCopies=3; scale=1; gap=1; break;
		case 4: numCopies=2; scale=1; gap=7; break;
		case 5: numCopies=1; scale=2; break;
		case 6: numCopies=3; scale=1; gap=3; break;
		case 7: numCopies=1; scale=4; break;
	}

	// clear
	for(int i=0 ; i<kHBlankClocks ; i++)
	{
		sprite1Bits[i] = false;
	}
	uint8_t val = GetGRP1();

	int offset = 0;

	for(int iCopy = 0 ; iCopy<numCopies ; iCopy++)
	{
		// sprite
		for(int bit=7 ; bit >= 0 ; bit--)
		{
			for(int rep=0 ; rep<scale ; rep++)
			{
				uint8_t i = resP1Pos+offset;
				if(i < kNumSpriteBits)
				{
					sprite1Bits[i] = val & (0x01 << bit);
				}
				offset++;
			}
		}
		// gap
		for(int iGap=0 ; iGap<gap ; iGap++)
		{
			for(int bit=7 ; bit >= 0 ; bit--)
			{
				uint8_t i = resP1Pos+offset;
				if(i < kNumSpriteBits)
				{
					sprite1Bits[resP1Pos+offset] = false;
				}
				offset++;
			}
		}
	}
}

uint8_t Tia::Read(uint8_t address)
{
	uint8_t ret = 0;
	
    switch(address)
    {
        case kCXM0P:
            ret = GetCXM0P();
			break;
        case kCXM1P:
            ret = GetCXM1P();
			break;
        case kCXP0FB:
            ret =  GetCXP0FB();
			break;
        case kCXP1FB:
            ret =  GetCXP1FB();
			break;
        case kCXM0FB:
            ret = GetCXM0FB();
			break;
        case kCXM1FB:
            ret = GetCXM1FB();
			break;
        case kCXBLPF:
            ret = GetCXBLPF();
			break;
        case kCXPPMM:
            ret = GetCXPPMM();
			break;
        case kINPT0:
            ret = GetINPT0();
			break;
        case kINPT1:
            ret = GetINPT1();
			break;
        case kINPT2:
            ret = GetINPT2();
			break;
        case kINPT3:
            ret = GetINPT3();
			break;
        case kINPT4:
            ret = GetINPT4();
			break;
        case kINPT5:
            ret = GetINPT5();
			break;
        default:
//            LOGERRORF("TIA::Unknown read addr 0x%02x", address);
            break;
    }
	if((address < kNumReadRegisters) && (bReadBreakpoints[address]))
	{
		Commands::Halt(true, Commands::HaltCommand::kHalt, "TIA read");		
	}
    return ret;
}

void Tia::Write(uint8_t address, uint8_t value)
{
    switch(address)
    {
        case kVSYNC:
            SetVSYNC(value);
            break;
        case kVBLANK:
            SetVBLANK(value);
            break;
        case kWSYNC:
            SetWSYNC(value);
            break;
        case kRSYNC:
            SetRSYNC(value);
            break;
        case kNUSIZ0:
            SetNUSIZ0(value);
            break;
        case kNUSIZ1:
            SetNUSIZ1(value);
            break;
        case kCOLUP0:
            SetCOLUP0(value);
            break;
        case kCOLUP1:
            SetCOLUP1(value);
            break;
        case kCOLUPF:
            SetCOLUPF(value);
            break;
        case kCOLUBK:
            SetCOLUBK(value);
            break;
        case kCTRLPF:
            SetCTRLPF(value);
            break;
        case kREFP0:
            SetREFP0(value);
            break;
        case kREFP1:
            SetREFP1(value);
            break;
        case kPF0:
            SetPF0(value);
            break;
        case kPF1:
            SetPF1(value);
            break;
        case kPF2:
            SetPF2(value);
            break;
        case kRESP0:
            SetRESP0(value);
            break;
        case kRESP1:
            SetRESP1(value);
            break;
        case kRESM0:
            SetRESM0(value);
            break;
        case kRESM1:
            SetRESM1(value);
            break;
        case kRESBL:
            SetRESBL(value);
            break;
        case kAUDC0:
            SetAUDC0(value);
            break;
        case kAUDC1:
            SetAUDC1(value);
            break;
        case kAUDF0:
            SetAUDF0(value);
            break;
        case kAUDF1:
            SetAUDF1(value);
            break;
        case kAUDV0:
            SetAUDV0(value);
            break;
        case kAUDV1:
            SetAUDV1(value);
            break;
        case kGRP0:
            SetGRP0(value);
            break;
        case kGRP1:
            SetGRP1(value);
            break;
        case kENAM0:
            SetENAM0(value);
            break;
        case kENAM1:
            SetENAM1(value);
            break;
        case kENABL:
            SetENABL(value);
            break;
        case kHMP0:
            SetHMP0(value);
            break;
        case kHMP1:
            SetHMP1(value);
            break;
        case kHMM0:
            SetHMM0(value);
            break;
        case kHMBL:
            SetHMBL(value);
            break;
        case kVDELP0:
            SetVDELP0(value);
            break;
        case kVDELP1:
            SetVDELP1(value);
            break;
        case kVDELBL:
            SetVDELBL(value);
            break;
        case kRESMP0:
            SetRESMP0(value);
            break;
        case kRESMP1:
            SetRESMP1(value);
            break;
        case kHMOVE:
            SetHMOVE(value);
            break;
        case kHMCLR:
            SetHMCLR(value);
            break;
        case kCXCLR:
            SetCXCLR(value);
            break;
        default:
            break;
    }
	if((address < kNumWriteRegisters) && (bWriteBreakpoints[address]))
	{
		Commands::Halt(true, Commands::HaltCommand::kHalt, "TIA write");		
	}
}

bool Tia::HandleCommand(const std::shared_ptr<CommandBase> command)
{
#if 0
	if(command.name == Commands::kHaltCommand)
	{
		if((command.payload == "false") && (command.payload2 == Commands::kHaltCommandTickTia))
		{
			bHaltOnTick = true;
		}
		if((command.payload == "false") && (command.payload2 == Commands::kHaltCommandHBlank))
		{
			bHaltOnHBlank = true;
		}
		if((command.payload == "false") && (command.payload2 == Commands::kHaltCommandVBlank))
		{
			bHaltOnVBlank = true;
		}
	}
#endif
	return false;
}

uint8_t Tia::GetCXM0P()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXM0P unimplemented");
#endif
    return readRegisters[kCXM0P];
}
uint8_t Tia::GetCXM1P()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXM1P unimplemented");
#endif
    return readRegisters[kCXM1P];
}
uint8_t Tia::GetCXP0FB()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXP0FB unimplemented");
#endif
    return readRegisters[kCXP0FB];
}
uint8_t Tia::GetCXP1FB()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXP1FB unimplemented");
#endif
    return readRegisters[kCXP1FB];
}
uint8_t Tia::GetCXM0FB()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXM0FB unimplemented");
#endif
    return readRegisters[kCXM0FB];
}
uint8_t Tia::GetCXM1FB()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXM1FB unimplemented");
#endif
    return readRegisters[kCXM1FB];
}
uint8_t Tia::GetCXBLPF()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXBLPF unimplemented");
#endif
    return readRegisters[kCXBLPF];
}
uint8_t Tia::GetCXPPMM()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXPPMM unimplemented");
#endif
    return readRegisters[kCXPPMM];
}
uint8_t Tia::GetINPT0()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::INPT0 unimplemented");
#endif
    return readRegisters[kINPT0];
}
uint8_t Tia::GetINPT1()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::INPT1 unimplemented");
#endif
    return readRegisters[kINPT1];
}
uint8_t Tia::GetINPT2()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::INPT2 unimplemented");
#endif
    return readRegisters[kINPT2];
}
uint8_t Tia::GetINPT3()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::INPT3 unimplemented");
#endif
    return readRegisters[kINPT3];
}
uint8_t Tia::GetINPT4()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::INPT4 unimplemented");
#endif
    return readRegisters[kINPT4];
}
uint8_t Tia::GetINPT5()
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::INPT5 unimplemented");
#endif
    return readRegisters[kINPT5];
}


uint8_t	Tia::HOffsetToRealOffset(uint8_t offset)
{
	if(offset < 8)
	{
		return offset + 8;
	}
	else
	{
		return offset - 8;
	}
}

void Tia::SetVSYNC(uint8_t val)
{
    writeRegisters[kVSYNC] = val;
	if(val & 0x02)
	{
		Commands::VSync();
		
		rasterY = 0;
		frameNum++;
		if(bHaltOnVBlank)
		{
			Commands::Halt(true, Commands::HaltCommand::kHalt, "TIA VSync");
			bHaltOnVBlank = false;
		}
	}
}

void Tia::SetVBLANK(uint8_t val)
{
	if(val != 0)
	{
		rasterCountLatched = rasterCount;
		rasterCount = 0;
	}
    writeRegisters[kVBLANK] = val;
}
void Tia::SetWSYNC(uint8_t val)
{
    writeRegisters[kWSYNC] = 0x01;
	bCpuWaitingForHsync = true;
}
void Tia::SetRSYNC(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::RSYNC unimplemented");
#endif
    writeRegisters[kRSYNC] = val;
}
void Tia::SetNUSIZ0(uint8_t val)
{
    writeRegisters[kNUSIZ0] = val;
}
void Tia::SetNUSIZ1(uint8_t val)
{
    writeRegisters[kNUSIZ1] = val;
}
void Tia::SetCOLUP0(uint8_t val)
{
    writeRegisters[kCOLUP0] = (val >> 1) & 0x7f;
}
void Tia::SetCOLUP1(uint8_t val)
{
    writeRegisters[kCOLUP1] = (val >> 1) & 0x7f;
}
void Tia::SetCOLUPF(uint8_t val)
{
    writeRegisters[kCOLUPF] = (val >> 1) & 0x7f;
}
void Tia::SetCOLUBK(uint8_t val)
{
    writeRegisters[kCOLUBK] = (val >> 1) & 0x7f;
}
void Tia::SetCTRLPF(uint8_t val)
{
    writeRegisters[kCTRLPF] = val;
	RebuildPlayfieldBits();
}
void Tia::SetREFP0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::REFP0 unimplemented");
#endif
    writeRegisters[kREFP0] = val;
}
void Tia::SetREFP1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::REFP1 unimplemented");
#endif
    writeRegisters[kREFP1] = val;
}
void Tia::SetPF0(uint8_t val)
{
    writeRegisters[kPF0] = val;
	RebuildPlayfieldBits();
}
void Tia::SetPF1(uint8_t val)
{
    writeRegisters[kPF1] = val;
	RebuildPlayfieldBits();
}
void Tia::SetPF2(uint8_t val)
{
    writeRegisters[kPF2] = val;
	RebuildPlayfieldBits();
}
void Tia::SetRESP0(uint8_t val)
{
	resP0Pos = rasterX - kHBlankClocks;
	resP0Pos += HOffsetToRealOffset(GetHMP0());
	resP0Pos += 4;
    writeRegisters[kRESP0] = resP0Pos;
	RebuildSprite0Bits();
}
void Tia::SetRESP1(uint8_t val)
{
	resP1Pos = rasterX - kHBlankClocks;
	resP1Pos += HOffsetToRealOffset(GetHMP1());
	resP1Pos += 4;
    writeRegisters[kRESP1] = resP1Pos;
	RebuildSprite1Bits();
}

void Tia::SetRESM0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::RESM0 unimplemented");
#endif
//	resM0Pos = rasterX - kHBlankClocks;
    writeRegisters[kRESM0] = resM0Pos;
}
void Tia::SetRESM1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::RESM1 unimplemented");
#endif
//	resM1Pos = rasterX - kHBlankClocks;
    writeRegisters[kRESM1] = resM1Pos;
}
void Tia::SetRESBL(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::RESBL unimplemented");
#endif
//	resBLPos = rasterX - kHBlankClocks;
    writeRegisters[kRESBL] = resBLPos;
}
void Tia::SetAUDC0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::AUDC0 unimplemented");
#endif
    writeRegisters[kAUDC0] = val;
}
void Tia::SetAUDC1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::AUDC1 unimplemented");
#endif
    writeRegisters[kAUDC1] = val;
}
void Tia::SetAUDF0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::AUDF0 unimplemented");
#endif
    writeRegisters[kAUDF0] = val;
}
void Tia::SetAUDF1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::AUDF1 unimplemented");
#endif
    writeRegisters[kAUDF1] = val;
}
void Tia::SetAUDV0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::AUDV0 unimplemented");
#endif
    writeRegisters[kAUDV0] = val;
}
void Tia::SetAUDV1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::AUDV1 unimplemented");
#endif
    writeRegisters[kAUDV1] = val;
}
void Tia::SetGRP0(uint8_t val)
{
    writeRegisters[kGRP0] = val;
	RebuildSprite0Bits();
}
void Tia::SetGRP1(uint8_t val)
{
    writeRegisters[kGRP1] = val;
	RebuildSprite1Bits();
}
void Tia::SetENAM0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::ENAM0 unimplemented");
#endif
    writeRegisters[kENAM0] = val;
}
void Tia::SetENAM1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::ENAM1 unimplemented");
#endif
    writeRegisters[kENAM1] = val;
}
void Tia::SetENABL(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::ENABL unimplemented");
#endif
    writeRegisters[kENABL] = val;
}
void Tia::SetHMP0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMP0 unimplemented");
#endif
    writeRegisters[kHMP0] = val;
}
void Tia::SetHMP1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMP1 unimplemented");
#endif
    writeRegisters[kHMP1] = val;
}
void Tia::SetHMM0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMM0 unimplemented");
#endif
    writeRegisters[kHMM0] = val;
}
void Tia::SetHMM1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMM1 unimplemented");
#endif
    writeRegisters[kHMM1] = val;
}
void Tia::SetHMBL(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMBL unimplemented");
#endif
    writeRegisters[kHMBL] = val;
}
void Tia::SetVDELP0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::VDELP0 unimplemented");
#endif
    writeRegisters[kVDELP0] = val;
}
void Tia::SetVDELP1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::VDELP1 unimplemented");
#endif
    writeRegisters[kVDELP1] = val;
}
void Tia::SetVDELBL(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::VDELBL unimplemented");
#endif
    writeRegisters[kVDELBL] = val;
}
void Tia::SetRESMP0(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::RESMP0 unimplemented");
#endif
    writeRegisters[kRESMP0] = val;
}
void Tia::SetRESMP1(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::RESMP1 unimplemented");
#endif
    writeRegisters[kRESMP1] = val;
}
void Tia::SetHMOVE(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMOVE unimplemented");
#endif
    writeRegisters[kHMOVE] = val;
}
void Tia::SetHMCLR(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::HMCLR unimplemented");
#endif
    writeRegisters[kHMCLR] = val;
}
void Tia::SetCXCLR(uint8_t val)
{
#if HALT_ON_UNIMPLEMENTED
	Commands::Halt(true, "", "TIA::CXCLR unimplemented");
#endif
    writeRegisters[kCXCLR] = val;
}
