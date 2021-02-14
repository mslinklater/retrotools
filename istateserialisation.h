#pragma once

#include "json.hpp"
using json = nlohmann::json;

class IStateSerialisation
{
public:
	virtual void SerialiseState(json& object) = 0;
	virtual void DeserialiseState(json& object) = 0;
};
