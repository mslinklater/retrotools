#include <stdio.h>
#include <inttypes.h>
#include "vcs.h"

struct vcs_info {
	enum eVCSMemory address;
	uint8_t			bitmask;
	char*			name;
	char*			description;
};

static struct vcs_info info[VCS_NUM];

static void vcs_addInfo(enum eVCSMemory mem, uint8_t bitmask, char* name, char* description)
{
	info[mem].address = mem;
	info[mem].bitmask = bitmask;
	info[mem].name = name;
	info[mem].description = description;
}

void vcs_Init(void)
{
	printf("vcs_Init()\n");
	// clear the array
	for(int i=0 ; i<VCS_NUM ; i++)
	{
		vcs_addInfo((enum eVCSMemory)i, 0, "UNKNOWN", "Unknown register");
	}

	vcs_addInfo(VSYNC, 0b00000010, "VSYNC", "0000 00x0 Vertical sync set-clear");
	vcs_addInfo(VBLANK, 0b11000010, "VBLANK", "xx00 00x0 Vertical blank set-clear");
	vcs_addInfo(WSYNC, 0, "WSYNC", "---- ---- Wait for horizontal blank");
	vcs_addInfo(RSYNC, 0, "RSYNC", "---- ---- Reset horizontal sync counter");
	vcs_addInfo(NUSIZ0, 0b00110111, "NUSIZ0", "00xx 0xxx Number-size player/missile 0");
	vcs_addInfo(NUSIZ1, 0b00110111, "NUSIZ1", "00xx 0xxx Number-size player/missile 1");
	vcs_addInfo(COLUP0, 0b11111110, "COLUP0", "xxxx xxx0 Color-luminance player 0");
	vcs_addInfo(COLUP1, 0b11111110, "COLUP1", "xxxx xxx0 Color-luminance player 1");
	vcs_addInfo(COLUPF, 0b11111110, "COLUPF", "xxxx xxx0 Color-luminance playfield");
	vcs_addInfo(COLUBK, 0b11111110, "COLUBK", "xxxx xxx0 Color-luminance background");
	vcs_addInfo(CTRLPF, 0b00110111, "CTRLPF", "00xx 0xxx Control playfield, ball, collisions");
	vcs_addInfo(REFP0, 0b00001000, "REFP0", "0000 x000 Reflection player 0");
	vcs_addInfo(REFP1, 0b00001000, "REFP1", "0000 x000 Reflection player 1");
	vcs_addInfo(PF0, 0b11110000, "PF0", "xxxx 0000 Playfield register byte 0");
	vcs_addInfo(PF1, 0b11111111, "PF1", "xxxx xxxx Playfield register byte 1");
	vcs_addInfo(PF2, 0b11111111, "PF2", "xxxx xxxx Playfield register byte 2");
	vcs_addInfo(RESP0, 0b00000000, "RESP0", "---- ---- Reset player 0");
	vcs_addInfo(RESP1, 0b00000000, "RESP1", "---- ---- Reset player 1");
	vcs_addInfo(RESM0, 0b00000000, "RESM0", "---- ---- Reset missile 0");
	vcs_addInfo(RESM1, 0b00000000, "RESM1", "---- ---- Reset missile 1");
	vcs_addInfo(RESBL, 0b00000000, "RESBL", "---- ---- Reset ball");
	vcs_addInfo(AUDC0, 0b00001111, "AUDC0", "0000 xxxx Audio control 0");
	vcs_addInfo(AUDC1, 0b00001111, "AUDC1", "0000 xxxx Audio control 1");
	vcs_addInfo(AUDF0, 0b00011111, "AUDF0", "000x xxxx Audio frequency 0");
	vcs_addInfo(AUDF1, 0b00011111, "AUDF1", "000x xxxx AUdio frequency 1");
	vcs_addInfo(AUDV0, 0b00001111, "AUDV0", "0000 xxxx Audio volume 0");
	vcs_addInfo(AUDV1, 0b00001111, "AUDV1", "0000 xxxx Audio volume 1");
	vcs_addInfo(GRP0, 0b11111111, "GRP0", "xxxx xxxx Graphics register player 0");
	vcs_addInfo(GRP1, 0b11111111, "GRP1", "xxxx xxxx Graphics register player 1");
	vcs_addInfo(ENAM0, 0b00000010, "ENAM0", "0000 00x0 Graphics enable missile 0");
	vcs_addInfo(ENAM1, 0b00000010, "ENAM1", "0000 00x0 Graphics enable missile 1");
	vcs_addInfo(ENABL, 0b00000010, "ENABL", "0000 00x0 Graphics enable ball");
	vcs_addInfo(HMP0, 0b11110000, "HMP0", "xxxx 0000 Horizontal motion player 0");
	vcs_addInfo(HMP1, 0b11110000, "HMP1", "xxxx 0000 Horizontal motion player 1");
	vcs_addInfo(HMM0, 0b11110000, "HMM0", "xxxx 0000 Horizontal motion missile 0");
	vcs_addInfo(HMM1, 0b11110000, "HMM1", "xxxx 0000 Horizontal motion missile 1");
	vcs_addInfo(HMBL, 0b11110000, "HMBL", "xxxx 0000 Horizontal motion ball");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
	vcs_addInfo(, 0b, "", "");
}
