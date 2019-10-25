#include <stdio.h>
#include <inttypes.h>
#include "vcs.h"

struct vcs_info {
	enum eVCSMemory address;
	char*			readName;
	char*			readDescription;
	char*			writeName;
	char*			writeDescription;
};

static struct vcs_info info[VCS_NUM];

static void vcs_addInfo(enum eVCSMemory mem, char* readName, char* readDescription, char* writeName, char* writeDescription)
{
	// NOTE - if either of the read or write name/description are null, the values should be the same as the other one
	info[mem].address = mem;
	info[mem].readName = readName;
	info[mem].readDescription = readDescription;
	info[mem].writeName = writeName;
	info[mem].writeDescription = writeDescription;
	
	// copy of null
	if(info[mem].readName == 0)
	{
		info[mem].readName = info[mem].writeName;
	}
	if(info[mem].readDescription == 0)
	{
		info[mem].readDescription = info[mem].writeDescription;
	}
	if(info[mem].writeName == 0)
	{
		info[mem].writeName = info[mem].readName;
	}
	if(info[mem].writeDescription == 0)
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
		vcs_addInfo((enum eVCSMemory)i, "UNKNOWN", "Unknown register", 0, 0);
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

	vcs_addInfo(PF1, 0, 0, "PF1", "xxxx xxxx Playfield register byte 1");
	vcs_addInfo(PF2, 0, 0, "PF2", "xxxx xxxx Playfield register byte 2");
	vcs_addInfo(RESP0, 0, 0, "RESP0", "---- ---- Reset player 0");
	vcs_addInfo(RESP1, 0, 0, "RESP1", "---- ---- Reset player 1");
	vcs_addInfo(RESM0, 0, 0, "RESM0", "---- ---- Reset missile 0");
	vcs_addInfo(RESM1, 0, 0, "RESM1", "---- ---- Reset missile 1");
	vcs_addInfo(RESBL, 0, 0, "RESBL", "---- ---- Reset ball");
	vcs_addInfo(AUDC0, 0, 0, "AUDC0", "0000 xxxx Audio control 0");
	vcs_addInfo(AUDC1, 0, 0, "AUDC1", "0000 xxxx Audio control 1");
	vcs_addInfo(AUDF0, 0, 0, "AUDF0", "000x xxxx Audio frequency 0");
	vcs_addInfo(AUDF1, 0, 0, "AUDF1", "000x xxxx AUdio frequency 1");
	vcs_addInfo(AUDV0, 0, 0, "AUDV0", "0000 xxxx Audio volume 0");
	vcs_addInfo(AUDV1, 0, 0, "AUDV1", "0000 xxxx Audio volume 1");
	vcs_addInfo(GRP0, 0, 0, "GRP0", "xxxx xxxx Graphics register player 0");
	vcs_addInfo(GRP1, 0, 0, "GRP1", "xxxx xxxx Graphics register player 1");
	vcs_addInfo(ENAM0, 0, 0, "ENAM0", "0000 00x0 Graphics enable missile 0");
	vcs_addInfo(ENAM1, 0, 0, "ENAM1", "0000 00x0 Graphics enable missile 1");
	vcs_addInfo(ENABL, 0, 0, "ENABL", "0000 00x0 Graphics enable ball");
	vcs_addInfo(HMP0, 0, 0, "HMP0", "xxxx 0000 Horizontal motion player 0");
	vcs_addInfo(HMP1, 0, 0, "HMP1", "xxxx 0000 Horizontal motion player 1");
	vcs_addInfo(HMM0, 0, 0, "HMM0", "xxxx 0000 Horizontal motion missile 0");
	vcs_addInfo(HMM1, 0, 0, "HMM1", "xxxx 0000 Horizontal motion missile 1");
	vcs_addInfo(HMBL, 0, 0, "HMBL", "xxxx 0000 Horizontal motion ball");
	vcs_addInfo(VDELP0, 0, 0, "VDELP0", "0000 000x Vertical delay player 0");
	vcs_addInfo(VDELP1, 0, 0, "VDELP1", "0000 000x Vertical delay player 1");
	vcs_addInfo(VDELBL, 0, 0, "VDELBL", "0000 000x Vertical delay ball");
	vcs_addInfo(RESMP0, 0, 0, "RESMP0", "0000 00x0 Reset missile 0 to player 0");
	vcs_addInfo(RESMP1, 0, 0, "RESMP1", "0000 00x0 Reset missile 1 to player 1");
	vcs_addInfo(HMOVE, 0, 0, "HMOVE", "---- ---- Apply horizontal motion");
	vcs_addInfo(HMCLR, 0, 0, "HMCLR", "---- ---- Clear horizontal move registers");
	vcs_addInfo(CXCLR, 0, 0, "CXCLR", "---- ---- Clear collision latches");
}
