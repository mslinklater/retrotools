// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class ITickable
{
    public:

        virtual void CommitInputs() = 0;		// commit state of input pins - so chip update order doesn't matter
        virtual void Tick(bool clockState) = 0;	// update the actual silicon state - based on the clockState
};

#define ITICKABLE_API                     \
	virtual void CommitInputs() override; \
	virtual void Tick(bool clockState) override;
