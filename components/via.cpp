#include "via.h"
#include "../log.h"

Via::Via()
{

}

Via::~Via()
{

}

uint8_t Via::Read(uint8_t address)
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

void Via::Write(uint8_t address, uint8_t value)
{
    switch(address)
    {
        default:
            break;
    }
}