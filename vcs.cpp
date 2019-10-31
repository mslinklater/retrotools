//#include <stdio.h>
//#include <inttypes.h>
#include "vcs.h"

Vcs::Vcs()
{
}

Vcs::~Vcs()
{
}

void Vcs::AddInfo(enum eVCSMemory mem, const std::string& readName, const std::string& readDescription, const std::string& writeName, const std::string& writeDescription)
{
	// NOTE - if either of the read or write name/description are null, the values should be the same as the other one
	info[mem].address = mem;
	info[mem].readName = readName;
	info[mem].readDescription = readDescription;
	info[mem].writeName = writeName;
	info[mem].writeDescription = writeDescription;
	
	// copy of null
	if(info[mem].readName.empty())
	{
		info[mem].readName = info[mem].writeName;
	}
	if(info[mem].readDescription.empty())
	{
		info[mem].readDescription = info[mem].writeDescription;
	}
	if(info[mem].writeName.empty())
	{
		info[mem].writeName = info[mem].readName;
	}
	if(info[mem].writeDescription.empty())
	{
		info[mem].writeDescription = info[mem].readDescription;
	}
}

void Vcs::Init(void)
{
	printf("vcs_Init()\n");

	// clear the array
	for(int i=0 ; i<VCS_NUM ; i++)
	{
		AddInfo((enum eVCSMemory)i, "", "", "", "");
	}

    // VSYNC & CXM0P
	AddInfo(VSYNC, "CXM0P", "xx00 0000 Read collision M0-P1 M0-P0", "VSYNC", "0000 00x0 Vertical sync set-clear");
    // VBLANK & CXM1P
	AddInfo(VBLANK, "CXM1P", "xx00 0000 Read collision M1-P0 M1-P1", "VBLANK", "xx00 00x0 Vertical blank set-clear");
    // WSYNC & CXP0FB
	AddInfo(WSYNC, "CXP0FB", "xx00 0000 Read collision P0-PF P0-BL", "WSYNC", "---- ---- Wait for horizontal blank");
    // RSYNC & CXP1FB
	AddInfo(RSYNC, "CXP1FB", "xx00 0000 Read collision P1-PF P1-BL", "RSYNC", "---- ---- Reset horizontal sync counter");
    // NUSIZ0 & CXM0FB
	AddInfo(NUSIZ0, "CXM0FB", "xx00 0000 Read collision M0-PF M0-BL", "NUSIZ0", "00xx 0xxx Number-size player/missile 0");
    // NUSIZ1 & CXM1FB
	AddInfo(NUSIZ1, "CXM1FB", "xx00 0000 Read collision M1-PF M1-BL", "NUSIZ1", "00xx 0xxx Number-size player/missile 1");
    // COLUP0 & CXBLPF
	AddInfo(COLUP0, "CXBLPF", "x000 0000 Read collision BL-PF", "COLUP0", "xxxx xxx0 Color-luminance player 0");
    // COLUP1 & CXPPMM
	AddInfo(COLUP1, "CXPPMM", "xx00 0000 Read collision P0-P1 M0-M1", "COLUP1", "xxxx xxx0 Color-luminance player 1");
    // COLUPF & INPT0
	AddInfo(COLUPF, "INPT0", "x000 0000 Read pot port 0", "COLUPF", "xxxx xxx0 Color-luminance playfield");
    // COLUBK & INPT1
	AddInfo(COLUBK, "INPT1", "x000 0000 Read pot port 1", "COLUBK", "xxxx xxx0 Color-luminance background");
    // CTRLPF & INPT2
	AddInfo(CTRLPF, "INPT2", "x000 0000 Read pot port 2", "CTRLPF", "00xx 0xxx Control playfield, ball, collisions");
    // REFP0 & INPT3
	AddInfo(REFP0, "INPT3", "x000 0000 Read pot port 3", "REFP0", "0000 x000 Reflection player 0");
    // REFP1 & INPT4
	AddInfo(REFP1, "INPT4", "Read input (trigger) 0", "REFP1", "0000 x000 Reflection player 1");
    // PF0 & INPT5
	AddInfo(PF0, "INPT5", "Read input (trigger) 1", "PF0", "xxxx 0000 Playfield register byte 0");

	AddInfo(PF1, "", "", "PF1", "xxxx xxxx Playfield register byte 1");
	AddInfo(PF2, "", "", "PF2", "xxxx xxxx Playfield register byte 2");
	AddInfo(RESP0, "", "", "RESP0", "---- ---- Reset player 0");
	AddInfo(RESP1, "", "", "RESP1", "---- ---- Reset player 1");
	AddInfo(RESM0, "", "", "RESM0", "---- ---- Reset missile 0");
	AddInfo(RESM1, "", "", "RESM1", "---- ---- Reset missile 1");
	AddInfo(RESBL, "", "", "RESBL", "---- ---- Reset ball");
	AddInfo(AUDC0, "", "", "AUDC0", "0000 xxxx Audio control 0");
	AddInfo(AUDC1, "", "", "AUDC1", "0000 xxxx Audio control 1");
	AddInfo(AUDF0, "", "", "AUDF0", "000x xxxx Audio frequency 0");
	AddInfo(AUDF1, "", "", "AUDF1", "000x xxxx AUdio frequency 1");
	AddInfo(AUDV0, "", "", "AUDV0", "0000 xxxx Audio volume 0");
	AddInfo(AUDV1, "", "", "AUDV1", "0000 xxxx Audio volume 1");
	AddInfo(GRP0, "", "", "GRP0", "xxxx xxxx Graphics register player 0");
	AddInfo(GRP1, "", "", "GRP1", "xxxx xxxx Graphics register player 1");
	AddInfo(ENAM0, "", "", "ENAM0", "0000 00x0 Graphics enable missile 0");
	AddInfo(ENAM1, "", "", "ENAM1", "0000 00x0 Graphics enable missile 1");
	AddInfo(ENABL, "", "", "ENABL", "0000 00x0 Graphics enable ball");
	AddInfo(HMP0, "", "", "HMP0", "xxxx 0000 Horizontal motion player 0");
	AddInfo(HMP1, "", "", "HMP1", "xxxx 0000 Horizontal motion player 1");
	AddInfo(HMM0, "", "", "HMM0", "xxxx 0000 Horizontal motion missile 0");
	AddInfo(HMM1, "", "", "HMM1", "xxxx 0000 Horizontal motion missile 1");
	AddInfo(HMBL, "", "", "HMBL", "xxxx 0000 Horizontal motion ball");
	AddInfo(VDELP0, "", "", "VDELP0", "0000 000x Vertical delay player 0");
	AddInfo(VDELP1, "", "", "VDELP1", "0000 000x Vertical delay player 1");
	AddInfo(VDELBL, "", "", "VDELBL", "0000 000x Vertical delay ball");
	AddInfo(RESMP0, "", "", "RESMP0", "0000 00x0 Reset missile 0 to player 0");
	AddInfo(RESMP1, "", "", "RESMP1", "0000 00x0 Reset missile 1 to player 1");
	AddInfo(HMOVE, "", "", "HMOVE", "---- ---- Apply horizontal motion");
	AddInfo(HMCLR, "", "", "HMCLR", "---- ---- Clear horizontal move registers");
	AddInfo(CXCLR, "", "", "CXCLR", "---- ---- Clear collision latches");
    // RIOT
	AddInfo(SWCHA, "SWCHA", "Port A data register for joysticks. 0-3 P1, 4-7 P2", "", "");
	AddInfo(SWACNT, "SWACNT", "Port A data direction register (DDR)", "", "");
	AddInfo(SWCHB, "SWCHB", "Port B DDR", "", "");
	AddInfo(INTIM, "INTIM", "Timer output", "", "");
	AddInfo(TIMINT, "TIMINT", "", "", "");
	AddInfo(TIM1T, "TIM1T", "Set 1 clock interval", "", "");
	AddInfo(TIM8T, "TIM8T", "Set 8 clock interval", "", "");
	AddInfo(TIM64T, "TIM64T", "Set 64 clock interval", "", "");
	AddInfo(T1024T, "T1024T", "Set 1024 clock interval", "", "");
}

const Vcs::MemoryInfo* Vcs::GetInfo(uint16_t address)
{
	if(address < VCS_NUM)
	{   
		return &info[address];
	}
	else
	{
		return 0;
	}
}

