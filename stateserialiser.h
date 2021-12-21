// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

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