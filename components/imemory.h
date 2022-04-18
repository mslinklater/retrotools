// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <inttypes.h>

class IMemory
{
    public:
        virtual uint8_t Read(uint16_t address) = 0;
        virtual void Write(uint16_t address, uint8_t value) = 0;

		// Dbg methods do not affect the state of the memory metadata - they are used for internal class access rather than emulated access
        virtual uint8_t DbgRead(uint16_t address) = 0;
        virtual void DbgWrite(uint16_t address, uint8_t value) = 0;

		virtual void SetHasBeenExecuted(uint16_t address, uint16_t length) = 0;
};

#define IMEMORY_API \
    virtual uint8_t Read(uint16_t address) override; \
    virtual void Write(uint16_t address, uint8_t value) override;    \
    virtual uint8_t DbgRead(uint16_t address) override;  \
    virtual void DbgWrite(uint16_t address, uint8_t value) override; \
    virtual void SetHasBeenExecuted(uint16_t address, uint16_t length) override;
