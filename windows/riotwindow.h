#pragma once

#include "windowbase.h"
#include "istateserialisation.h"
#include "../components/stella/riot.h"

class RiotWindow : public WindowBase, public IStateSerialisation
{
public:
	RiotWindow();
	virtual ~RiotWindow();
	
	virtual void Draw(void);

    void SetRiot(Riot* riot);

	// IStateSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IStateSerialisation

private:
    Riot* pRiot;

	void DrawRegister(uint32_t reg, const char* text, uint8_t val);
};
