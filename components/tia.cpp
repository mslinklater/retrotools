#include "tia.h"
#include "../shared_cpp/log.h"
#include "../commands.h"
#include "../shared_cpp/command.h"

Tia::Tia()
: rasterX(0)
, rasterY(0)
, frameNum(0)
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

	InitPalettes();

	for(int y=0 ; y<kOutputVerticalResolution ; y++)
	{
		for(int x=0 ; x<kOutputHorizontalResolution ; x++)
		{
			pixels[(y*kOutputHorizontalResolution) + x] = y & 7;
		}
	}

	// Playfield bits
	for(int i=0 ; i<40 ; i++)
	{
		playfieldBits[i] = false;
	}
}

Tia::~Tia()
{

}

void Tia::InitPalettes()
{
	// NTSC

}

void Tia::Tick()
{
	// do tick

	if((GetVSYNC() & 0x02) != 0) 	// D1 of VSYNC
	{
		rasterY = 0;
	}

	// Begin actual render logic
	pixels[rasterX + (rasterY*228)] = GetCOLUBK();
	if(rasterX >= 68)
	{
		int playfieldIndex = (rasterX-68)/4;
		if(playfieldBits[playfieldIndex])
		{
			pixels[rasterX + (rasterY*228)] = GetCOLUPF();
		}
	}
	// End actual render logic

	// now move
	rasterX++;
	if(rasterX >= 228)
	{
		// hblank
		rasterX = 0;
		rasterY++;
		if(bHaltOnHBlank)
		{
			Commands::Halt(true);
			bHaltOnHBlank = false;
		}
		bCpuWaitingForHsync = false;
		// end hblank

		if(rasterY >= 262)
		{
			// vblank
			rasterY = 0;
			frameNum++;
			if(bHaltOnVBlank)
			{
				Commands::Halt(true);
				bHaltOnVBlank = false;
			}
		}
	}

	ticksSinceBoot++;

	if(bHaltOnTick)
	{
		Commands::Halt(true);
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

uint8_t Tia::Read(uint8_t address)
{
    switch(address)
    {
        case kCXM0P:
            return GetCXM0P();
        case kCXM1P:
            return GetCXM1P();
        case kCXP0FB:
            return GetCXP0FB();
        case kCXP1FB:
            return GetCXP1FB();
        case kCXM0FB:
            return GetCXM0FB();
        case kCXM1FB:
            return GetCXM1FB();
        case kCXBLPF:
            return GetCXBLPF();
        case kCXPPMM:
            return GetCXPPMM();
        case kINPT0:
            return GetINPT0();
        case kINPT1:
            return GetINPT1();
        case kINPT2:
            return GetINPT2();
        case kINPT3:
            return GetINPT3();
        case kINPT4:
            return GetINPT4();
        case kINPT5:
            return GetINPT5();
        default:
            LOGERRORF("VIA::Unknown read addr 0x%02x", address);
            break;
    }
    return 0;
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
            SetCOLUBK((value >> 1) & 0x7f);
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
	if(bWriteBreakpoints[address])
	{
		Commands::Halt(true);		
	}
}

bool Tia::HandleCommand(const Command& command)
{
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
	return false;
}

uint8_t Tia::GetCXM0P()
{
    return readRegisters[kCXM0P];
}
uint8_t Tia::GetCXM1P()
{
    return readRegisters[kCXM1P];
}
uint8_t Tia::GetCXP0FB()
{
    return readRegisters[kCXP0FB];
}
uint8_t Tia::GetCXP1FB()
{
    return readRegisters[kCXP1FB];
}
uint8_t Tia::GetCXM0FB()
{
    return readRegisters[kCXM0FB];
}
uint8_t Tia::GetCXM1FB()
{
    return readRegisters[kCXM1FB];
}
uint8_t Tia::GetCXBLPF()
{
    return readRegisters[kCXBLPF];
}
uint8_t Tia::GetCXPPMM()
{
    return readRegisters[kCXPPMM];
}
uint8_t Tia::GetINPT0()
{
    return readRegisters[kINPT0];
}
uint8_t Tia::GetINPT1()
{
    return readRegisters[kINPT1];
}
uint8_t Tia::GetINPT2()
{
    return readRegisters[kINPT2];
}
uint8_t Tia::GetINPT3()
{
    return readRegisters[kINPT3];
}
uint8_t Tia::GetINPT4()
{
    return readRegisters[kINPT4];
}
uint8_t Tia::GetINPT5()
{
    return readRegisters[kINPT5];
}
void Tia::SetVSYNC(uint8_t val)
{
    writeRegisters[kVSYNC] = val;
}

void Tia::SetVBLANK(uint8_t val)
{
    writeRegisters[kVBLANK] = val;    
}
void Tia::SetWSYNC(uint8_t val)
{
    writeRegisters[kWSYNC] = 0x01;
	bCpuWaitingForHsync = true;
}
void Tia::SetRSYNC(uint8_t val)
{
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
    writeRegisters[kCOLUP0] = val;
}
void Tia::SetCOLUP1(uint8_t val)
{
    writeRegisters[kCOLUP1] = val;
}
void Tia::SetCOLUPF(uint8_t val)
{
    writeRegisters[kCOLUPF] = val;
}
void Tia::SetCOLUBK(uint8_t val)
{
    writeRegisters[kCOLUBK] = val;
}
void Tia::SetCTRLPF(uint8_t val)
{
    writeRegisters[kCTRLPF] = val;
	RebuildPlayfieldBits();
}
void Tia::SetREFP0(uint8_t val)
{
    writeRegisters[kREFP0] = val;
}
void Tia::SetREFP1(uint8_t val)
{
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
    writeRegisters[kRESP0] = val;
}
void Tia::SetRESP1(uint8_t val)
{
    writeRegisters[kRESP1] = val;
}
void Tia::SetRESM0(uint8_t val)
{
    writeRegisters[kRESM0] = val;
}
void Tia::SetRESM1(uint8_t val)
{
    writeRegisters[kRESM1] = val;
}
void Tia::SetRESBL(uint8_t val)
{
    writeRegisters[kRESBL] = val;
}
void Tia::SetAUDC0(uint8_t val)
{
    writeRegisters[kAUDC0] = val;
}
void Tia::SetAUDC1(uint8_t val)
{
    writeRegisters[kAUDC1] = val;
}
void Tia::SetAUDF0(uint8_t val)
{
    writeRegisters[kAUDF0] = val;
}
void Tia::SetAUDF1(uint8_t val)
{
    writeRegisters[kAUDF1] = val;
}
void Tia::SetAUDV0(uint8_t val)
{
    writeRegisters[kAUDV0] = val;
}
void Tia::SetAUDV1(uint8_t val)
{
    writeRegisters[kAUDV1] = val;
}
void Tia::SetGRP0(uint8_t val)
{
    writeRegisters[kGRP0] = val;
}
void Tia::SetGRP1(uint8_t val)
{
    writeRegisters[kGRP1] = val;
}
void Tia::SetENAM0(uint8_t val)
{
    writeRegisters[kENAM0] = val;
}
void Tia::SetENAM1(uint8_t val)
{
    writeRegisters[kENAM1] = val;
}
void Tia::SetENABL(uint8_t val)
{
    writeRegisters[kENABL] = val;
}
void Tia::SetHMP0(uint8_t val)
{
    writeRegisters[kHMP0] = val;
}
void Tia::SetHMP1(uint8_t val)
{
    writeRegisters[kHMP1] = val;
}
void Tia::SetHMM0(uint8_t val)
{
    writeRegisters[kHMM0] = val;
}
void Tia::SetHMM1(uint8_t val)
{
    writeRegisters[kHMM1] = val;
}
void Tia::SetHMBL(uint8_t val)
{
    writeRegisters[kHMBL] = val;
}
void Tia::SetVDELP0(uint8_t val)
{
    writeRegisters[kVDELP0] = val;
}
void Tia::SetVDELP1(uint8_t val)
{
    writeRegisters[kVDELP1] = val;
}
void Tia::SetVDELBL(uint8_t val)
{
    writeRegisters[kVDELBL] = val;
}
void Tia::SetRESMP0(uint8_t val)
{
    writeRegisters[kRESMP0] = val;
}
void Tia::SetRESMP1(uint8_t val)
{
    writeRegisters[kRESMP1] = val;
}
void Tia::SetHMOVE(uint8_t val)
{
    writeRegisters[kHMOVE] = val;
}
void Tia::SetHMCLR(uint8_t val)
{
    writeRegisters[kHMCLR] = val;
}
void Tia::SetCXCLR(uint8_t val)
{
    writeRegisters[kCXCLR] = val;
}
