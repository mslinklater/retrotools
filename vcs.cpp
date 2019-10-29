//#include <stdio.h>
//#include <inttypes.h>
#include "vcs.h"

static struct vcs_info info[VCS_NUM];

static void vcs_addInfo(enum eVCSMemory mem, const std::string& readName, const std::string& readDescription, const std::string& writeName, const std::string& writeDescription)
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

void vcs_Init(void)
{
	printf("vcs_Init()\n");

	// clear the array
	for(int i=0 ; i<VCS_NUM ; i++)
	{
		vcs_addInfo((enum eVCSMemory)i, "", "", "", "");
	}

    // VSYNC & CXM0P
	vcs_addInfo(VSYNC, "CXM0P", "xx00 0000 Read collision M0-P1 M0-P0", "VSYNC", "0000 00x0 Vertical sync set-clear");
    // VBLANK & CXM1P
	vcs_addInfo(VBLANK, "CXM1P", "xx00 0000 Read collision M1-P0 M1-P1", "VBLANK", "xx00 00x0 Vertical blank set-clear");
    // WSYNC & CXP0FB
	vcs_addInfo(WSYNC, "CXP0FB", "xx00 0000 Read collision P0-PF P0-BL", "WSYNC", "---- ---- Wait for horizontal blank");
    // RSYNC & CXP1FB
	vcs_addInfo(RSYNC, "CXP1FB", "xx00 0000 Read collision P1-PF P1-BL", "RSYNC", "---- ---- Reset horizontal sync counter");
    // NUSIZ0 & CXM0FB
	vcs_addInfo(NUSIZ0, "CXM0FB", "xx00 0000 Read collision M0-PF M0-BL", "NUSIZ0", "00xx 0xxx Number-size player/missile 0");
    // NUSIZ1 & CXM1FB
	vcs_addInfo(NUSIZ1, "CXM1FB", "xx00 0000 Read collision M1-PF M1-BL", "NUSIZ1", "00xx 0xxx Number-size player/missile 1");
    // COLUP0 & CXBLPF
	vcs_addInfo(COLUP0, "CXBLPF", "x000 0000 Read collision BL-PF", "COLUP0", "xxxx xxx0 Color-luminance player 0");
    // COLUP1 & CXPPMM
	vcs_addInfo(COLUP1, "CXPPMM", "xx00 0000 Read collision P0-P1 M0-M1", "COLUP1", "xxxx xxx0 Color-luminance player 1");
    // COLUPF & INPT0
	vcs_addInfo(COLUPF, "INPT0", "x000 0000 Read pot port 0", "COLUPF", "xxxx xxx0 Color-luminance playfield");
    // COLUBK & INPT1
	vcs_addInfo(COLUBK, "INPT1", "x000 0000 Read pot port 1", "COLUBK", "xxxx xxx0 Color-luminance background");
    // CTRLPF & INPT2
	vcs_addInfo(CTRLPF, "INPT2", "x000 0000 Read pot port 2", "CTRLPF", "00xx 0xxx Control playfield, ball, collisions");
    // REFP0 & INPT3
	vcs_addInfo(REFP0, "INPT3", "x000 0000 Read pot port 3", "REFP0", "0000 x000 Reflection player 0");
    // REFP1 & INPT4
	vcs_addInfo(REFP1, "INPT4", "Read input (trigger) 0", "REFP1", "0000 x000 Reflection player 1");
    // PF0 & INPT5
	vcs_addInfo(PF0, "INPT5", "Read input (trigger) 1", "PF0", "xxxx 0000 Playfield register byte 0");

	vcs_addInfo(PF1, "", "", "PF1", "xxxx xxxx Playfield register byte 1");
	vcs_addInfo(PF2, "", "", "PF2", "xxxx xxxx Playfield register byte 2");
	vcs_addInfo(RESP0, "", "", "RESP0", "---- ---- Reset player 0");
	vcs_addInfo(RESP1, "", "", "RESP1", "---- ---- Reset player 1");
	vcs_addInfo(RESM0, "", "", "RESM0", "---- ---- Reset missile 0");
	vcs_addInfo(RESM1, "", "", "RESM1", "---- ---- Reset missile 1");
	vcs_addInfo(RESBL, "", "", "RESBL", "---- ---- Reset ball");
	vcs_addInfo(AUDC0, "", "", "AUDC0", "0000 xxxx Audio control 0");
	vcs_addInfo(AUDC1, "", "", "AUDC1", "0000 xxxx Audio control 1");
	vcs_addInfo(AUDF0, "", "", "AUDF0", "000x xxxx Audio frequency 0");
	vcs_addInfo(AUDF1, "", "", "AUDF1", "000x xxxx AUdio frequency 1");
	vcs_addInfo(AUDV0, "", "", "AUDV0", "0000 xxxx Audio volume 0");
	vcs_addInfo(AUDV1, "", "", "AUDV1", "0000 xxxx Audio volume 1");
	vcs_addInfo(GRP0, "", "", "GRP0", "xxxx xxxx Graphics register player 0");
	vcs_addInfo(GRP1, "", "", "GRP1", "xxxx xxxx Graphics register player 1");
	vcs_addInfo(ENAM0, "", "", "ENAM0", "0000 00x0 Graphics enable missile 0");
	vcs_addInfo(ENAM1, "", "", "ENAM1", "0000 00x0 Graphics enable missile 1");
	vcs_addInfo(ENABL, "", "", "ENABL", "0000 00x0 Graphics enable ball");
	vcs_addInfo(HMP0, "", "", "HMP0", "xxxx 0000 Horizontal motion player 0");
	vcs_addInfo(HMP1, "", "", "HMP1", "xxxx 0000 Horizontal motion player 1");
	vcs_addInfo(HMM0, "", "", "HMM0", "xxxx 0000 Horizontal motion missile 0");
	vcs_addInfo(HMM1, "", "", "HMM1", "xxxx 0000 Horizontal motion missile 1");
	vcs_addInfo(HMBL, "", "", "HMBL", "xxxx 0000 Horizontal motion ball");
	vcs_addInfo(VDELP0, "", "", "VDELP0", "0000 000x Vertical delay player 0");
	vcs_addInfo(VDELP1, "", "", "VDELP1", "0000 000x Vertical delay player 1");
	vcs_addInfo(VDELBL, "", "", "VDELBL", "0000 000x Vertical delay ball");
	vcs_addInfo(RESMP0, "", "", "RESMP0", "0000 00x0 Reset missile 0 to player 0");
	vcs_addInfo(RESMP1, "", "", "RESMP1", "0000 00x0 Reset missile 1 to player 1");
	vcs_addInfo(HMOVE, "", "", "HMOVE", "---- ---- Apply horizontal motion");
	vcs_addInfo(HMCLR, "", "", "HMCLR", "---- ---- Clear horizontal move registers");
	vcs_addInfo(CXCLR, "", "", "CXCLR", "---- ---- Clear collision latches");
    // RIOT
	vcs_addInfo(SWCHA, "SWCHA", "Port A data register for joysticks. 0-3 P1, 4-7 P2", "", "");
	vcs_addInfo(SWACNT, "SWACNT", "Port A data direction register (DDR)", "", "");
	vcs_addInfo(SWCHB, "SWCHB", "Port B DDR", "", "");
	vcs_addInfo(INTIM, "INTIM", "Timer output", "", "");
	vcs_addInfo(TIMINT, "TIMINT", "", "", "");
	vcs_addInfo(TIM1T, "TIM1T", "Set 1 clock interval", "", "");
	vcs_addInfo(TIM8T, "TIM8T", "Set 8 clock interval", "", "");
	vcs_addInfo(TIM64T, "TIM64T", "Set 64 clock interval", "", "");
	vcs_addInfo(T1024T, "T1024T", "Set 1024 clock interval", "", "");
}

const struct vcs_info* vcs_getInfo(uint16_t address)
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

