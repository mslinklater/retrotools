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
            return GetCXM0P();
        case kCXM1P:
            return GetCXM1P();
        case kCXP0FB:
            return GetCXP0FB();
        case kCXP1FB:
            return GetCXP1FB();
        case kCXM0FB:
            return GetCXM0FB();
        case kCXM1FB:
            return GetCXM1FB();
        case kCXBLPF:
            return GetCXBLPF();
        case kCXPPMM:
            return GetCXPPMM();
        case kINPT0:
            return GetINPT0();
        case kINPT1:
            return GetINPT1();
        case kINPT2:
            return GetINPT2();
        case kINPT3:
            return GetINPT3();
        case kINPT4:
            return GetINPT4();
        case kINPT5:
            return GetINPT5();
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
            SetVSYNC(value);
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
            LOGERRORF("Unknown TIA write 0x%02x", address);
            break;
    }
}

uint8_t Tia::GetCXM0P()
{
    return CXM0P;
}
uint8_t Tia::GetCXM1P()
{
    return CXM1P;
}
uint8_t Tia::GetCXP0FB()
{
    return CXP0FB;
}
uint8_t Tia::GetCXP1FB()
{
    return CXP1FB;
}
uint8_t Tia::GetCXM0FB()
{
    return CXM0FB;
}
uint8_t Tia::GetCXM1FB()
{
    return CXM1FB;
}
uint8_t Tia::GetCXBLPF()
{
    return CXM1FB;
}
uint8_t Tia::GetCXPPMM()
{
    return CXPPMM;
}
uint8_t Tia::GetINPT0()
{
    return INPT0;
}
uint8_t Tia::GetINPT1()
{
    return INPT1;
}
uint8_t Tia::GetINPT2()
{
    return INPT2;
}
uint8_t Tia::GetINPT3()
{
    return INPT3;
}
uint8_t Tia::GetINPT4()
{
    return INPT4;
}
uint8_t Tia::GetINPT5()
{
    return INPT5;
}
void Tia::SetVSYNC(uint8_t val)
{

}
void Tia::SetVBLANK(uint8_t val)
{
    
}
void Tia::SetWSYNC(uint8_t val)
{
    
}
void Tia::SetRSYNC(uint8_t val)
{
    
}
void Tia::SetNUSIZ0(uint8_t val)
{
    
}
void Tia::SetNUSIZ1(uint8_t val)
{
    
}
void Tia::SetCOLUP0(uint8_t val)
{
    
}
void Tia::SetCOLUP1(uint8_t val)
{
    
}
void Tia::SetCOLUPF(uint8_t val)
{
    
}
void Tia::SetCOLUBK(uint8_t val)
{
    
}
void Tia::SetCTRLPF(uint8_t val)
{
    
}
void Tia::SetREFP0(uint8_t val)
{
    
}
void Tia::SetREFP1(uint8_t val)
{
    
}
void Tia::SetPF0(uint8_t val)
{
    
}
void Tia::SetPF1(uint8_t val)
{
    
}
void Tia::SetPF2(uint8_t val)
{
    
}
void Tia::SetRESP0(uint8_t val)
{
    
}
void Tia::SetRESP1(uint8_t val)
{
    
}
void Tia::SetRESM0(uint8_t val)
{
    
}
void Tia::SetRESM1(uint8_t val)
{
    
}
void Tia::SetRESBL(uint8_t val)
{
    
}
void Tia::SetAUDC0(uint8_t val)
{
    
}
void Tia::SetAUDC1(uint8_t val)
{
    
}
void Tia::SetAUDF0(uint8_t val)
{
    
}
void Tia::SetAUDF1(uint8_t val)
{
    
}
void Tia::SetAUDV0(uint8_t val)
{
    
}
void Tia::SetAUDV1(uint8_t val)
{
    
}
void Tia::SetGRP0(uint8_t val)
{
    
}
void Tia::SetGRP1(uint8_t val)
{
    
}
void Tia::SetENAM0(uint8_t val)
{
    
}
void Tia::SetENAM1(uint8_t val)
{
    
}
void Tia::SetENABL(uint8_t val)
{
    
}
void Tia::SetHMP0(uint8_t val)
{
    
}
void Tia::SetHMP1(uint8_t val)
{
    
}
void Tia::SetHMM0(uint8_t val)
{
    
}
void Tia::SetHMBL(uint8_t val)
{
    
}
void Tia::SetVDELP0(uint8_t val)
{
    
}
void Tia::SetVDELP1(uint8_t val)
{
    
}
void Tia::SetVDELBL(uint8_t val)
{
    
}
void Tia::SetRESMP0(uint8_t val)
{
    
}
void Tia::SetRESMP1(uint8_t val)
{
    
}
void Tia::SetHMOVE(uint8_t val)
{
    
}
void Tia::SetHMCLR(uint8_t val)
{
    
}
void Tia::SetCXCLR(uint8_t val)
{
    
}
