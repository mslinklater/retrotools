// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include "istateserialisation.h"

class StateSerialiser
{
public:
	void AddStateSerialiser(std::shared_ptr<IStateSerialisation> serialiser);
	void SerialiseAppConfig();
	void DeserialiseAppConfig();
private:
	std::vector<std::shared_ptr<IStateSerialisation>> serialisers;
};
