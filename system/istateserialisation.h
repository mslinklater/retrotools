// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include "json.hpp"
using json = nlohmann::json;

class IStateSerialisation
{
public:
	virtual void SerialiseState(json& object) = 0;
	virtual void DeserialiseState(json& object) = 0;
};

#define ISERIALISATION_API                     \
	virtual void SerialiseState(json &object); \
	virtual void DeserialiseState(json &object);
