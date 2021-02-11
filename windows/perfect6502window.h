#pragma once

#include "../shared_cpp/windowbase.h"
#include "../shared_cpp/istateserialisation.h"
#include "../perfect6502/perfect6502.h"

class Perfect6502Window : public WindowBase, public IStateSerialisation
{
public:
	Perfect6502Window();
	virtual ~Perfect6502Window();

	virtual void Draw();

	// IStateSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IStateSerialisation

private:

	bool bRunning;

	state_t* pState;
};