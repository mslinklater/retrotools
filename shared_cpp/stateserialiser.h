#pragma once

#include <vector>
#include "istateserialisation.h"

class StateSerialiser
{
public:
	void AddStateSerialiser(IStateSerialisation* serialiser);
	void SerialiseAppConfig();
	void DeserialiseAppConfig();
private:
	std::vector<IStateSerialisation*> serialisers;
};