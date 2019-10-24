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
		vcs_addInfo((enum eVCSMemory)i, 0, 0, 0);
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
}
