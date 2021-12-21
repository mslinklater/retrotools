// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "symbol.h"

bool Symbol::Serialise(json& object)
{
    // check for flags - strip hardware
    if(flags & kSymbolFlag_Hardware)
    {
        // hardware symbols are already defined 
        return false;
    }

    object["address"] = address;
    if(labelName.length() > 0)
    {
        object["labelName"] = labelName;
    }
    if(readName.length() > 0)
    {
        object["readName"] = readName;
    }
    if(writeName.length() > 0)
    {
        object["writeName"] = writeName;
    }
    object["flag_memoryread"] = flags & kSymbolFlag_MemoryRead ? true : false;
    object["flag_memorywrite"] = flags & kSymbolFlag_MemoryWrite ? true : false;
    object["flag_addresslabel"] = flags & kSymbolFlag_AddressLabel ? true : false;
    object["flag_hardware"] = flags & kSymbolFlag_Hardware ? true : false;
    object["flag_user"] = flags & kSymbolFlag_User ? true : false;
    object["flag_auto"] = flags & kSymbolFlag_Auto ? true : false;
    object["flag_jumplabel"] = flags & kSymbolFlag_JumpLabel ? true : false;
    object["flag_branchlabel"] = flags & kSymbolFlag_BranchLabel ? true : false;

    return true;
}

void Symbol::Deserialise(json& object)
{
//    object["plap"].object();
    address = object["address"].get<uint16_t>();

    if(object.contains("labelName"))
    {
        labelName = object["labelName"].get<std::string>();
    }
    if(object.contains("readName"))
    {
        readName = object["readName"].get<std::string>();
    }
    if(object.contains("writeName"))
    {
        writeName = object["writeName"].get<std::string>();
    }
    flags = 0;
    if(object.contains("flag_memoryread"))
    {
        if(object["flag_memoryread"].get<bool>()) flags |= kSymbolFlag_MemoryRead;
    }
    if(object.contains("flag_memorywrite"))
    {
        if(object["flag_memorywrite"].get<bool>()) flags |= kSymbolFlag_MemoryWrite;
    }
    if(object.contains("flag_addresslabel"))
    {
        if(object["flag_addresslabel"].get<bool>()) flags |= kSymbolFlag_AddressLabel;
    }
    if(object.contains("flag_hardware"))
    {
        if(object["flag_hardware"].get<bool>()) flags |= kSymbolFlag_Hardware;
    }
    if(object.contains("flag_user"))
    {
        if(object["flag_user"].get<bool>()) flags |= kSymbolFlag_User;
    }
    if(object.contains("flag_auto"))
    {
        if(object["flag_auto"].get<bool>()) flags |= kSymbolFlag_Auto;
    }
    if(object.contains("flag_jumplabel"))
    {
        if(object["flag_jumplabel"].get<bool>()) flags |= kSymbolFlag_JumpLabel;
    }
    if(object.contains("flag_branchlabel"))
    {
        if(object["flag_branchlabel"].get<bool>()) flags |= kSymbolFlag_BranchLabel;
    }
}
