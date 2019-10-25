// Atari 2600 memory hardware mappings

enum eVCSMemory {
	VSYNC = 0,	// Write
	VBLANK,
	WSYNC,
	RSYNC,
	NUSIZ0,
	NUSIZ1,
	COLUP0,
	COLUP1,
	COLUPF,
	COLUBK,
	CTRLPF,
	REFP0,
	REFP1,
	PF0,
	PF1,
	PF2,
	RESP0,
	RESP1,
	RESM0,
	RESM1,
	RESBL,
	AUDC0,
	AUDC1,
	AUDF0,
	AUDF1,
	AUDV0,
	AUDV1,
	GRP0,
	GRP1,
	ENAM0,
	ENAM1,
	ENABL,
	HMP0,
	HMP1,
	HMM0,
	HMM1,
	HMBL,
	VDELP0,
	VDELP1,
	VDELBL,
	RESMP0,
	RESMP1,
	HMOVE,
	HMCLR,
	CXCLR,

	CXM0P = 0,	// Read
	CXM1P,
	CXP0FB,
	CXP1FB,
	CXM0FB,
	CXM1FB,
	CXBLPF,
	CXPPMM,
	INPT0,
	INPT1,
	INPT2,
	INPT3,
	INPT4,
	INPT5,

	SWCHA = 0x280,	// RIOT
	SWACNT,
	SWCHB,
	SWBCNT,
	INTIM,
	TIMINT,
	TIM1T = 0x294,
	TIM8T,
	TIM64T,
	T1024T,

	VCS_NUM
};

struct vcs_info {
	enum eVCSMemory address;
	char*			readName;
	char*			readDescription;
	char*			writeName;
	char*			writeDescription;
};

extern void vcs_Init(void);
extern const struct vcs_info* vcs_getInfo(uint16_t address);

