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

class Via
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
    static const uint8_t kCOUPF = 0x08;
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

    Via();
    virtual ~Via();

    uint8_t Read(uint8_t address);
    void    Write(uint8_t address, uint8_t value);

private:
    //uint8_t pixels[228*262];

    // palette
};
