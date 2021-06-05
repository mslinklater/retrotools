#pragma once
/* Timing info

Each horizontal raster is 228 VIA clocks
262 rasters in full frame

3 rasters V-Sync
37 rasters V-Blank
192 rasters of image (NTSC)
242 rasters of image (PAL)
30 rasters of overscan

Each line consists of

68 clocks h-blank
160 clocks of image

@60fps = 3584160 Hz clock

---

HSprite adjust

value	dec	2scomp adjust
0b0000	0	0		8
0b0001	1	1		9
0b0010	2	2		10
0b0011	3	3		11
0b0100	4	4		12
0b0101	5	5		13
0b0110	6	6		14
0b0111	7	7		15
0b1000	8	-8		0
0b1001	9	-7		1
0b1010	10	-6		2
0b1011	11	-5		3
0b1100	12	-4		4
0b1101	13	-3		5
0b1110	14	-2		6
0b1111	15	-1		7
*/

#include <inttypes.h>
#include "../interfaces/itickable.h"
#include "../icommandhandler.h"

class Tia : public ITickable, public ICommandHandler
{
public:

	enum ERegion {
		NTSC,
		PAL,
		SECAM
	};

	static const uint16_t kOutputHorizontalResolution = 228;
	static const uint16_t kOutputVerticalResolution = 313;	// big enough for PAL
	static const uint8_t kHBlankClocks = 68;

	// read registers
    static const uint8_t kCXM0P = 0x00;
    static const uint8_t kCXM1P = 0x01;
    static const uint8_t kCXP0FB = 0x02;
    static const uint8_t kCXP1FB = 0x03;
    static const uint8_t kCXM0FB = 0x04;
    static const uint8_t kCXM1FB = 0x05;
    static const uint8_t kCXBLPF = 0x06;
    static const uint8_t kCXPPMM = 0x07;
    static const uint8_t kINPT0 = 0x08;
    static const uint8_t kINPT1 = 0x09;
    static const uint8_t kINPT2 = 0x0a;
    static const uint8_t kINPT3 = 0x0b;
    static const uint8_t kINPT4 = 0x0c;
    static const uint8_t kINPT5 = 0x0d;
	static const uint8_t kNumReadRegisters = 0x0e;

	// write registers
    static const uint8_t kVSYNC = 0x00;
    static const uint8_t kVBLANK = 0x01;
    static const uint8_t kWSYNC = 0x02;
    static const uint8_t kRSYNC = 0x03;
    static const uint8_t kNUSIZ0 = 0x04;
    static const uint8_t kNUSIZ1 = 0x05;
    static const uint8_t kCOLUP0 = 0x06;
    static const uint8_t kCOLUP1 = 0x07;
    static const uint8_t kCOLUPF = 0x08;
    static const uint8_t kCOLUBK = 0x09;
    static const uint8_t kCTRLPF = 0x0a;
    static const uint8_t kREFP0 = 0x0b;
    static const uint8_t kREFP1 = 0x0c;
    static const uint8_t kPF0 = 0x0d;
    static const uint8_t kPF1 = 0x0e;
    static const uint8_t kPF2 = 0x0f;
    static const uint8_t kRESP0 = 0x10;
    static const uint8_t kRESP1 = 0x11;
    static const uint8_t kRESM0 = 0x12;
    static const uint8_t kRESM1 = 0x13;
    static const uint8_t kRESBL = 0x14;
    static const uint8_t kAUDC0 = 0x15;
    static const uint8_t kAUDC1 = 0x16;
    static const uint8_t kAUDF0 = 0x17;
    static const uint8_t kAUDF1 = 0x18;
    static const uint8_t kAUDV0 = 0x19;
    static const uint8_t kAUDV1 = 0x1a;
    static const uint8_t kGRP0 = 0x1b;
    static const uint8_t kGRP1 = 0x1c;
    static const uint8_t kENAM0 = 0x1d;
    static const uint8_t kENAM1 = 0x1e;
    static const uint8_t kENABL = 0x1f;
    static const uint8_t kHMP0 = 0x20;
    static const uint8_t kHMP1 = 0x21;
    static const uint8_t kHMM0 = 0x22;
    static const uint8_t kHMM1 = 0x23;
    static const uint8_t kHMBL = 0x24;
    static const uint8_t kVDELP0 = 0x25;
    static const uint8_t kVDELP1 = 0x26;
    static const uint8_t kVDELBL = 0x27;
    static const uint8_t kRESMP0 = 0x28;
    static const uint8_t kRESMP1 = 0x29;
    static const uint8_t kHMOVE = 0x2a;
    static const uint8_t kHMCLR = 0x2b;
    static const uint8_t kCXCLR = 0x2c;
	static const uint8_t kNumWriteRegisters = 0x2d;

    Tia();
    virtual ~Tia();

    uint8_t Read(uint8_t address);
    void    Write(uint8_t address, uint8_t value);

	bool	GetReadBreakpoint(uint8_t reg){return bReadBreakpoints[reg];}
	void	SetReadBreakpoint(uint8_t reg, bool state){bReadBreakpoints[reg] = state;}
	bool	GetWriteBreakpoint(uint8_t reg){return bWriteBreakpoints[reg];}
	void	SetWriteBreakpoint(uint8_t reg, bool state){bWriteBreakpoints[reg] = state;}

	bool	IsCpuStalled(){return bCpuWaitingForHsync;}

	uint16_t GetRasterX(){return rasterX;}
	uint16_t GetRasterY(){return rasterY;}
	uint32_t GetFrameNum(){return frameNum;}

    // Read only registers
    uint8_t GetCXM0P();
    uint8_t GetCXM1P();
    uint8_t GetCXP0FB();
    uint8_t GetCXP1FB();
    uint8_t GetCXM0FB();
    uint8_t GetCXM1FB();
    uint8_t GetCXBLPF();
    uint8_t GetCXPPMM();
    uint8_t GetINPT0();
    uint8_t GetINPT1();
    uint8_t GetINPT2();
    uint8_t GetINPT3();
    uint8_t GetINPT4();
    uint8_t GetINPT5();

    // Write only registers
    void SetVSYNC(uint8_t val);
    void SetVBLANK(uint8_t val);
    void SetWSYNC(uint8_t val);
    void SetRSYNC(uint8_t val);
    void SetNUSIZ0(uint8_t val);
    void SetNUSIZ1(uint8_t val);
    void SetCOLUP0(uint8_t val);
    void SetCOLUP1(uint8_t val);
    void SetCOLUPF(uint8_t val);
    void SetCOLUBK(uint8_t val);
    void SetCTRLPF(uint8_t val);
    void SetREFP0(uint8_t val);
    void SetREFP1(uint8_t val);
    void SetPF0(uint8_t val);
    void SetPF1(uint8_t val);
    void SetPF2(uint8_t val);
    void SetRESP0(uint8_t val);
    void SetRESP1(uint8_t val);
    void SetRESM0(uint8_t val);
    void SetRESM1(uint8_t val);
    void SetRESBL(uint8_t val);
    void SetAUDC0(uint8_t val);
    void SetAUDC1(uint8_t val);
    void SetAUDF0(uint8_t val);
    void SetAUDF1(uint8_t val);
    void SetAUDV0(uint8_t val);
    void SetAUDV1(uint8_t val);
    void SetGRP0(uint8_t val);
    void SetGRP1(uint8_t val);
    void SetENAM0(uint8_t val);
    void SetENAM1(uint8_t val);
    void SetENABL(uint8_t val);
    void SetHMP0(uint8_t val);
    void SetHMP1(uint8_t val);
    void SetHMM0(uint8_t val);
    void SetHMM1(uint8_t val);
    void SetHMBL(uint8_t val);
    void SetVDELP0(uint8_t val);
    void SetVDELP1(uint8_t val);
    void SetVDELBL(uint8_t val);
    void SetRESMP0(uint8_t val);
    void SetRESMP1(uint8_t val);
    void SetHMOVE(uint8_t val);
    void SetHMCLR(uint8_t val);
    void SetCXCLR(uint8_t val);

	uint64_t GetTicksSinceBoot(){return ticksSinceBoot;}

    // Read access to write only registers
    uint8_t GetVSYNC(void){return writeRegisters[kVSYNC];}
    uint8_t GetVBLANK(void){return writeRegisters[kVBLANK];}
    uint8_t GetRSYNC(void){return writeRegisters[kRSYNC];}
    uint8_t GetWSYNC(void){return writeRegisters[kWSYNC];}
    uint8_t GetNUSIZ0(void){return writeRegisters[kNUSIZ0];}
    uint8_t GetNUSIZ1(void){return writeRegisters[kNUSIZ1];}
    uint8_t GetCOLUP0(void){return writeRegisters[kCOLUP0];}
    uint8_t GetCOLUP1(void){return writeRegisters[kCOLUP1];}
    uint8_t GetCOLUPF(void){return writeRegisters[kCOLUPF];}
    uint8_t GetCOLUBK(void){return writeRegisters[kCOLUBK];}
    uint8_t GetREFP0(void){return writeRegisters[kREFP0];}
    uint8_t GetREFP1(void){return writeRegisters[kREFP1];}
    uint8_t GetPF0(void){return writeRegisters[kPF0];}
    uint8_t GetPF1(void){return writeRegisters[kPF1];}
    uint8_t GetPF2(void){return writeRegisters[kPF2];}
    uint8_t GetRESP0(void){return writeRegisters[kRESP0];}
    uint8_t GetRESP1(void){return writeRegisters[kRESP1];}
    uint8_t GetRESM0(void){return writeRegisters[kRESM0];}
    uint8_t GetRESM1(void){return writeRegisters[kRESM1];}
    uint8_t GetRESBL(void){return writeRegisters[kRESBL];}
    uint8_t GetAUDC0(void){return writeRegisters[kAUDC0];}
    uint8_t GetAUDC1(void){return writeRegisters[kAUDC1];}
    uint8_t GetAUDF0(void){return writeRegisters[kAUDF0];}
    uint8_t GetAUDF1(void){return writeRegisters[kAUDF1];}
    uint8_t GetAUDV0(void){return writeRegisters[kAUDV0];}
    uint8_t GetAUDV1(void){return writeRegisters[kAUDV1];}
    uint8_t GetGRP0(void){return writeRegisters[kGRP0];}
    uint8_t GetGRP1(void){return writeRegisters[kGRP1];}
    uint8_t GetENAM0(void){return writeRegisters[kENAM0];}
    uint8_t GetENAM1(void){return writeRegisters[kENAM1];}
    uint8_t GetENABL(void){return writeRegisters[kENABL];}
    uint8_t GetHMP0(void){return writeRegisters[kHMP0];}
    uint8_t GetHMP1(void){return writeRegisters[kHMP1];}
    uint8_t GetHMM0(void){return writeRegisters[kHMM0];}
    uint8_t GetHMM1(void){return writeRegisters[kHMM1];}
    uint8_t GetHMBL(void){return writeRegisters[kHMBL];}
    uint8_t GetVDELP0(void){return writeRegisters[kVDELP0];}
    uint8_t GetVDELP1(void){return writeRegisters[kVDELP1];}
    uint8_t GetVDELBL(void){return writeRegisters[kVDELBL];}
    uint8_t GetRESMP0(void){return writeRegisters[kRESMP0];}
    uint8_t GetRESMP1(void){return writeRegisters[kRESMP1];}
    uint8_t GetHMOVE(void){return writeRegisters[kHMOVE];}
    uint8_t GetHMCLR(void){return writeRegisters[kHMCLR];}
    uint8_t GetCXCLR(void){return writeRegisters[kCXCLR];}

    // ITickable
	void CommitInputs(){}		// commit state of input pins - so chip update order doesn't matter
	void Tick(bool clockState){}	// update the actual silicon state - based on the clockState
    // ~ITickable

    void Tick();

	// ICommandProcessor
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command);
	// ~ICommandProcessor

	const uint8_t* 	GetPixels(){return &pixels[0];}
	const bool* 	GetCpuActive(){return &cpuActive[0];}
	const bool*		GetVBlankActive(){return &vblankActive[0];}
	const uint8_t* 	GetPalette();

	ERegion	GetRegion(){return region;}
	void SetRegion(ERegion newRegion){region = newRegion;}
	uint16_t GetRasterCount(){return rasterCountLatched;}

	void SetShowPF(bool showPF){bShowPF = showPF;}
	void SetShowP0(bool showP0){bShowP0 = showP0;}
	void SetShowP1(bool showP1){bShowP1 = showP1;}

private:
	void RebuildPlayfieldBits();
	void RebuildSprite0Bits();
	void RebuildSprite1Bits();

	uint8_t	HOffsetToRealOffset(uint8_t offset);

	// Sprite hpos
	uint8_t	resP0Pos;
	uint8_t resP1Pos;
	uint8_t resM0Pos;
	uint8_t resM1Pos;
	uint8_t resBLPos;

	bool bShowPF;
	bool bShowP0;
	bool bShowP1;

    uint8_t 	pixels[kOutputHorizontalResolution*kOutputVerticalResolution];
	bool		vblankActive[kOutputVerticalResolution];
	bool		cpuActive[kOutputHorizontalResolution*kOutputVerticalResolution];
	uint16_t	rasterX;
	uint16_t	rasterY;
	uint32_t	frameNum;
	uint16_t	rasterCount;
	uint16_t	rasterCountLatched;

	ERegion		region;

	static const uint8_t kNumColours = 128;
	uint8_t		paletteNTSC[kNumColours*3] = {
		0x00, 0x00, 0x00,	0x40, 0x40, 0x40,	0x6c, 0x6c, 0x6c,	0x90, 0x90, 0x90,	0xb0, 0xb0, 0xb0,	0xc8, 0xc8, 0xc8,	0xdc, 0xdc, 0xdc,	0xec, 0xec, 0xec,
		0x44, 0x44, 0x00,	0x64, 0x64, 0x10,	0x84, 0x84, 0x24,	0xa0, 0xa0, 0x34,	0xb8, 0xb8, 0x40,	0xd0, 0xd0, 0x50,	0xe8, 0xe8, 0x5c,	0xfc, 0xfc, 0x68,
		0x70, 0x28, 0x00,	0x84, 0x44, 0x14,	0x98, 0x5c, 0x28,	0xac, 0x78, 0x3c,	0xbc, 0x8c, 0x4c,	0xcc, 0xa0, 0x5c,	0xdc, 0xb4, 0x68,	0xec, 0xc8, 0x78,
		0x84, 0x18, 0x00,	0x98, 0x34, 0x18,	0xac, 0x50, 0x30,	0xc0, 0x68, 0x48,	0xd0, 0x80, 0x5c,	0xe0, 0x94, 0x70,	0xec, 0xa8, 0x80,	0xfc, 0xbc, 0x94,
		0x88, 0x00, 0x00,	0x9c, 0x20, 0x20,	0xb0, 0x3c, 0x3c,	0xc0, 0x58, 0x58,	0xd0, 0x70, 0x70,	0xe0, 0x88, 0x88,	0xec, 0xa0, 0xa0,	0xfc, 0xb4, 0xb4,
		0x78, 0x00, 0x5c,	0x8c, 0x20, 0x74,	0xa0, 0x3c, 0x88,	0xb0, 0x58, 0x9c,	0xc0, 0x70, 0xb0,	0xd0, 0x84, 0xc0,	0xdc, 0x9c, 0xd0,	0xec, 0xb0, 0xe0,
		0x48, 0x00, 0x78,	0x60, 0x20, 0x90,	0x78, 0x3c, 0xa4,	0x8c, 0x58, 0xb8,	0xa0, 0x70, 0xcc,	0xb4, 0x84, 0xdc,	0xc4, 0x9c, 0xec,	0xd4, 0xb0, 0xfc,
		0x14, 0x00, 0x84,	0x30, 0x20, 0x98,	0x4c, 0x3c, 0xac,	0x68, 0x58, 0xc0,	0x7c, 0x70, 0xd0,	0x94, 0x88, 0xe0,	0xa8, 0xa0, 0xec,	0xbc, 0xb4, 0xfc,
		0x00, 0x00, 0x88,	0x12, 0x20, 0x9c,	0x38, 0x40, 0xb0,	0x50, 0x5c, 0xc0,	0x68, 0x74, 0xd0,	0x7c, 0x8c, 0xe0,	0x90, 0xa4, 0xec,	0xa4, 0xb8, 0xfc,
		0x00, 0x18, 0x7c,	0x1c, 0x38, 0x90,	0x38, 0x54, 0xa8,	0x50, 0x70, 0xbc,	0x68, 0x88, 0xcc,	0x7c, 0x9c, 0xdc,	0x90, 0xb4, 0xec,	0xa4, 0xc8, 0xfc,
		0x00, 0x2c, 0x5c,	0x1c, 0x4c, 0x78,	0x38, 0x68, 0x90,	0x50, 0x84, 0xac,	0x68, 0x9c, 0xc0,	0x7c, 0xb4, 0xd4,	0x90, 0xcc, 0xe8,	0xa4, 0xe0, 0xfc,
		0x00, 0x3c, 0x2c,	0x1c, 0x5c, 0x48,	0x38, 0x7c, 0x64,	0x50, 0x9c, 0x80,	0x68, 0xb4, 0x94,	0x7c, 0xd0, 0xac,	0x90, 0xe4, 0xc0,	0xa4, 0xfc, 0xd4,
		0x00, 0x3c, 0x00,	0x20, 0x5c, 0x20,	0x40, 0x7c, 0x40,	0x5c, 0x9c, 0x5c,	0x74, 0xb4, 0x74,	0x8c, 0xd0, 0x8c,	0xa4, 0xe4, 0xa4,	0xb8, 0xfc, 0xb8,
		0x14, 0x38, 0x00,	0x34, 0x5c, 0x1c,	0x50, 0x7c, 0x38,	0x6c, 0x98, 0x50,	0x84, 0xb4, 0x68,	0x9c, 0xcc, 0x7c,	0xb4, 0xe4, 0x90,	0xc8, 0xfc, 0xa4,
		0x2c, 0x30, 0x00,	0x4c, 0x50, 0x1c,	0x68, 0x70, 0x34,	0x84, 0x8c, 0x4c,	0x9c, 0xa8, 0x64,	0xb4, 0xc0, 0x78,	0xcc, 0xd4, 0x88,	0xe0, 0xec, 0x9c,
		0x44, 0x28, 0x00,	0x64, 0x48, 0x18,	0x84, 0x68, 0x30,	0xa0, 0x84, 0x44,	0xb8, 0x9c, 0x58,	0xd0, 0xb4, 0x6c,	0xe8, 0xcc, 0x7c,	0xfc, 0xe0, 0x8c

	};
	uint8_t		palettePAL[kNumColours*3] = {
		0x00, 0x00, 0x00,	0x28, 0x28, 0x28,	0x50, 0x50, 0x50,	0x74, 0x74, 0x74,	0x94, 0x94, 0x94,	0xb4, 0xb4, 0xb4,	0xd0, 0xd0, 0xd0,	0xec, 0xec, 0xec,
		0x00, 0x00, 0x00,	0x28, 0x28, 0x28,	0x50, 0x50, 0x50,	0x74, 0x74, 0x74,	0x94, 0x94, 0x94,	0xb4, 0xb4, 0xb4,	0xd0, 0xd0, 0xd0,	0xec, 0xec, 0xec,

		0x80, 0x58, 0x00,	0x94, 0x70, 0x20,	0xa8, 0x84, 0x3c,	0xbc, 0x9c, 0x58,	0xcc, 0xac, 0x70,	0xdc, 0xc0, 0x84,	0xec, 0xd0, 0x9c,	0xfc, 0xe0, 0xb0,
		0x44, 0x5c, 0x00,	0x5c, 0x78, 0x20,	0x74, 0x90, 0x3c,	0x8c, 0xac, 0x58,	0xa0, 0xc0, 0x70,	0xb0, 0xd4, 0x84,	0xc4, 0xe9, 0x9c,	0xd4, 0xfc, 0xb0,
		0x70, 0x34, 0x00,	0x88, 0x50, 0x20,	0xa0, 0x68, 0x3c,	0xb4, 0x84, 0x58,	0xc8, 0x98, 0x70,	0xdc, 0xac, 0x84,	0xec, 0xc0, 0x9c,	0xfc, 0xd4, 0xb0,
		0x00, 0x64, 0x14,	0x20, 0x80, 0x34,	0x3c, 0x98, 0x50,	0x58, 0xb0, 0x6c,	0x70, 0xc4, 0x84,	0x84, 0xd8, 0x9c,	0x9c, 0xe8, 0xb4,	0xb0, 0xfc, 0xc8,
		0x70, 0x00, 0x14,	0x88, 0x20, 0x34,	0xa0, 0x3c, 0x50,	0xb4, 0x58, 0x6c,	0xc8, 0x70, 0x84,	0xdc, 0x84, 0x9c,	0xec, 0x9c, 0xb4,	0xfc, 0xb0, 0xc8,
		0x00, 0x5c, 0x5c,	0x20, 0x74, 0x74,	0x3c, 0x8c, 0x8c,	0x58, 0xa4, 0xa4,	0x70, 0xb8, 0xb8,	0x84, 0xc8, 0xc8,	0x9c, 0xdc, 0xdc,	0xb0, 0xec, 0xec,
		0x70, 0x00, 0x5c,	0x84, 0x20, 0x74,	0x94, 0x3c, 0x88,	0xa8, 0x58, 0x9c,	0xb4, 0x70, 0xb0,	0xc4, 0x84, 0xc0,	0xd0, 0x9c, 0xd0,	0xe0, 0xb0, 0xe0,
		0x00, 0x3c, 0x70,	0x1c, 0x58, 0x88,	0x38, 0x74, 0xa0,	0x50, 0x8c, 0xb4,	0x68, 0xa4, 0xc8,	0x7c, 0xb8, 0xdc,	0x90, 0xcc, 0xec,	0xa4, 0xe0, 0xfc,
		0x58, 0x00, 0x70,	0x6c, 0x20, 0x88,	0x80, 0x3c, 0xa0,	0x94, 0x58, 0xb4,	0xa4, 0x70, 0xc8,	0xb4, 0x84, 0xdc,	0xc4, 0x9c, 0xec,	0xd4, 0xb0, 0xfc,
		0x00, 0x20, 0x70,	0x1c, 0x3c, 0x88,	0x38, 0x58, 0xa0,	0x50, 0x74, 0xb4,	0x68, 0x88, 0xc8,	0x7c, 0xa0, 0xdc,	0x90, 0xb4, 0xec,	0xa4, 0xc8, 0xfc,
		0x3c, 0x00, 0x80,	0x54, 0x20, 0x94,	0x6c, 0x3c, 0xa8,	0x80, 0x58, 0xbc,	0x94, 0x70, 0xcc,	0xa8, 0x84, 0xdc,	0xb8, 0x9c, 0xec,	0xc8, 0xb0, 0xfc,
		0x00, 0x00, 0x88,	0x20, 0x20, 0x9c,	0x3c, 0x3c, 0xb0,	0x58, 0x58, 0xc0,	0x70, 0x70, 0xd0,	0x84, 0x84, 0xe0,	0x9c, 0x9c, 0xec,	0xb0, 0xb0, 0xfc,

		0x00, 0x00, 0x00,	0x28, 0x28, 0x28,	0x50, 0x50, 0x50,	0x74, 0x74, 0x74,	0x94, 0x94, 0x94,	0xb4, 0xb4, 0xb4,	0xd0, 0xd0, 0xd0,	0xec, 0xec, 0xec,
		0x00, 0x00, 0x00,	0x28, 0x28, 0x28,	0x50, 0x50, 0x50,	0x74, 0x74, 0x74,	0x94, 0x94, 0x94,	0xb4, 0xb4, 0xb4,	0xd0, 0xd0, 0xd0,	0xec, 0xec, 0xec,
	};

	uint8_t		paletteSECAM[kNumColours*3] = {
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff,
		0x00, 0x00, 0x00,	0x21, 0x21, 0xff,	0xf0, 0x3c, 0x79,	0xff, 0x50, 0xff,	0x7f, 0xff, 0x00,	0x7f, 0xff, 0xff,	0xff, 0xff, 0x3f,	0xff, 0xff, 0xff
	};

	bool bReadBreakpoints[kNumReadRegisters];
	bool bWriteBreakpoints[kNumWriteRegisters];

	uint8_t readRegisters[kNumReadRegisters];
	uint8_t writeRegisters[kNumWriteRegisters];

	static const int kNumPlayfieldBits = 40;
	bool playfieldBits[kNumPlayfieldBits];	// TODO - pack

	static const int kNumSpriteBits = 160;
	bool sprite0Bits[kNumSpriteBits];
	bool sprite1Bits[kNumSpriteBits];

	bool bHaltOnTick;
	bool bHaltOnHBlank;
	bool bHaltOnVBlank;
	bool bCpuWaitingForHsync;

	uint64_t	ticksSinceBoot;
};
