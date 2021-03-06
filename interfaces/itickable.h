#pragma once

class ITickable
{
    public:

        virtual void CommitInputs() = 0;		// commit state of input pins - so chip update order doesn't matter
        virtual void Tick(bool clockState) = 0;	// update the actual silicon state - based on the clockState
};
