#pragma once
/* Timing info

Each horizontal raster is 228 VIA clocks
262 rasters in full frame

3 rasters V-Sync
37 rasters V-Blank
192 rasters of image
30 rasters of overscan

Each line consists of

68 clocks h-blank
160 clocks of image

@60fps = 3584160 Hz clock

*/

#include <inttypes.h>
#include "../interfaces/itickable.h"
#include "../shared_cpp/icommandprocessor.h"

class Tia : public ITickable, public ICommandProcessor
{
public:

    static const uint8_t kVSYNC = 0x00;
    static const uint8_t kCXM0P = 0x00;
    static const uint8_t kVBLANK = 0x01;
    static const uint8_t kCXM1P = 0x01;
    static const uint8_t kWSYNC = 0x02;
    static const uint8_t kCXP0FB = 0x02;
    static const uint8_t kRSYNC = 0x03;
    static const uint8_t kCXP1FB = 0x03;
    static const uint8_t kNUSIZ0 = 0x04;
    static const uint8_t kCXM0FB = 0x04;
    static const uint8_t kNUSIZ1 = 0x05;
    static const uint8_t kCXM1FB = 0x05;
    static const uint8_t kCOLUP0 = 0x06;
    static const uint8_t kCXBLPF = 0x06;
    static const uint8_t kCOLUP1 = 0x07;
    static const uint8_t kCXPPMM = 0x07;
    static const uint8_t kCOLUPF = 0x08;
    static const uint8_t kINPT0 = 0x08;
    static const uint8_t kCOLUBK = 0x09;
    static const uint8_t kINPT1 = 0x09;
    static const uint8_t kCTRLPF = 0x0a;
    static const uint8_t kINPT2 = 0x0a;
    static const uint8_t kREFP0 = 0x0b;
    static const uint8_t kINPT3 = 0x0b;
    static const uint8_t kREFP1 = 0x0c;
    static const uint8_t kINPT4 = 0x0c;
    static const uint8_t kPF0 = 0x0d;
    static const uint8_t kINPT5 = 0x0d;
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

    Tia();
    virtual ~Tia();

    uint8_t Read(uint8_t address);
    void    Write(uint8_t address, uint8_t value);

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

    // Read access to write only registers
    uint8_t GetVSYNC(void){return VSYNC;}
    uint8_t GetVBLANK(void){return VBLANK;}
    uint8_t GetRSYNC(void){return RSYNC;}
    uint8_t GetWSYNC(void){return WSYNC;}
    uint8_t GetNUSIZ0(void){return NUSIZ0;}
    uint8_t GetNUSIZ1(void){return NUSIZ1;}
    uint8_t GetCOLUP0(void){return COLUP0;}
    uint8_t GetCOLUP1(void){return COLUP1;}
    uint8_t GetCOLUPF(void){return COLUPF;}
    uint8_t GetCOLUBK(void){return COLUBK;}
    uint8_t GetREFP0(void){return REFP0;}
    uint8_t GetREFP1(void){return REFP1;}
    uint8_t GetPF0(void){return PF0;}
    uint8_t GetPF1(void){return PF1;}
    uint8_t GetPF2(void){return PF2;}
    uint8_t GetRESP0(void){return RESP0;}
    uint8_t GetRESP1(void){return RESP1;}
    uint8_t GetRESM0(void){return RESM0;}
    uint8_t GetRESM1(void){return RESM1;}
    uint8_t GetRESBL(void){return RESBL;}
    uint8_t GetAUDC0(void){return AUDC0;}
    uint8_t GetAUDC1(void){return AUDC1;}
    uint8_t GetAUDF0(void){return AUDF0;}
    uint8_t GetAUDF1(void){return AUDF1;}
    uint8_t GetAUDV0(void){return AUDV0;}
    uint8_t GetAUDV1(void){return AUDV1;}
    uint8_t GetGRP0(void){return GRP0;}
    uint8_t GetGRP1(void){return GRP1;}
    uint8_t GetENAM0(void){return ENAM0;}
    uint8_t GetENAM1(void){return ENAM1;}
    uint8_t GetENABL(void){return ENABL;}
    uint8_t GetHMP0(void){return HMP0;}
    uint8_t GetHMP1(void){return HMP1;}
    uint8_t GetHMM0(void){return HMM0;}
    uint8_t GetHMM1(void){return HMM1;}
    uint8_t GetHMBL(void){return HMBL;}
    uint8_t GetVDELP0(void){return VDELP0;}
    uint8_t GetVDELP1(void){return VDELP1;}
    uint8_t GetVDELBL(void){return VDELBL;}
    uint8_t GetRESMP0(void){return RESMP0;}
    uint8_t GetRESMP1(void){return RESMP1;}
    uint8_t GetHMOVE(void){return HMOVE;}
    uint8_t GetHMCLR(void){return HMCLR;}
    uint8_t GetCXCLR(void){return CXCLR;}

    // ITickable
    void Tick();
    // ~ITickable

	// ICommandProcessor
	virtual bool HandleCommand(const Command& command);
	// ~ICommandProcessor

private:
//    uint8_t pixels[228*262];

    // registers
    uint8_t VSYNC;
    uint8_t CXM0P;
    uint8_t VBLANK;
    uint8_t CXM1P;
    uint8_t WSYNC;
    uint8_t CXP0FB;
    uint8_t RSYNC;
    uint8_t CXP1FB;
    uint8_t NUSIZ0;
    uint8_t CXM0FB;
    uint8_t NUSIZ1;
    uint8_t CXM1FB;
    uint8_t COLUP0;
    uint8_t CXBLPF;
    uint8_t COLUP1;
    uint8_t CXPPMM;
    uint8_t COLUPF;
    uint8_t INPT0;
    uint8_t COLUBK;
    uint8_t INPT1;
    uint8_t CTRLPF;
    uint8_t INPT2;
    uint8_t REFP0;
    uint8_t INPT3;
    uint8_t REFP1;
    uint8_t INPT4;
    uint8_t PF0;
    uint8_t INPT5;
    uint8_t PF1;
    uint8_t PF2;
    uint8_t RESP0;
    uint8_t RESP1;
    uint8_t RESM0;
    uint8_t RESM1;
    uint8_t RESBL;
    uint8_t AUDC0;
    uint8_t AUDC1;
    uint8_t AUDF0;
    uint8_t AUDF1;
    uint8_t AUDV0;
    uint8_t AUDV1;
    uint8_t GRP0;
    uint8_t GRP1;
    uint8_t ENAM0;
    uint8_t ENAM1;
    uint8_t ENABL;
    uint8_t HMP0;
    uint8_t HMP1;
    uint8_t HMM0;
    uint8_t HMM1;
    uint8_t HMBL;
    uint8_t VDELP0;
    uint8_t VDELP1;
    uint8_t VDELBL;
    uint8_t RESMP0;
    uint8_t RESMP1;
    uint8_t HMOVE;
    uint8_t HMCLR;
    uint8_t CXCLR;
};
