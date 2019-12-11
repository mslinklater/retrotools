#include "tia.h"
#include "../log.h"

Tia::Tia()
{

}

Tia::~Tia()
{

}

uint8_t Tia::Read(uint8_t address)
{
    switch(address)
    {
        case kCXM0P:
            break;
        case kCXM1P:
            break;
        case kCXP0FB:
            break;
        case kCXP1FB:
            break;
        case kCXM0FB:
            break;
        case kCXM1FB:
            break;
        case kCXBLPF:
            break;
        case kCXPPMM:
            break;
        case kINPT0:
            break;
        case kINPT1:
            break;
        case kINPT2:
            break;
        case kINPT3:
            break;
        case kINPT4:
            break;
        case kINPT5:
            break;
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
            break;
        case kVBLANK:
            break;
        case kWSYNC:
            break;
        case kRSYNC:
            break;
        case kNUSIZ0:
            break;
        case kNUSIZ1:
            break;
        case kCOLUP0:
            break;
        case kCOLUP1:
            break;
        case kCOLUPF:
            break;
        case kCOLUBK:
            break;
        case kCTRLPF:
            break;
        case kREFP0:
            break;
        case kREFP1:
            break;
        case kPF0:
            break;
        case kPF1:
            break;
        case kPF2:
            break;
        case kRESP0:
            break;
        case kRESP1:
            break;
        case kRESM0:
            break;
        case kRESM1:
            break;
        case kRESBL:
            break;
        case kAUDC0:
            break;
        case kAUDC1:
            break;
        case kAUDF0:
            break;
        case kAUDF1:
            break;
        case kAUDV0:
            break;
        case kAUDV1:
            break;
        case kGRP0:
            break;
        case kGRP1:
            break;
        case kENAM0:
            break;
        case kENAM1:
            break;
        case kENABL:
            break;
        case kHMP0:
            break;
        case kHMP1:
            break;
        case kHMM0:
            break;
        case kHMBL:
            break;
        case kVDELP0:
            break;
        case kVDELP1:
            break;
        case kVDELBL:
            break;
        case kRESMP0:
            break;
        case kRESMP1:
            break;
        case kHMOVE:
            break;
        case kHMCLR:
            break;
        case kCXCLR:
            break;
        default:
            LOGERRORF("Unknown TIA write 0x%02x", value);
            break;
    }
}