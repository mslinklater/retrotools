// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <cstdint>
#include <vector>

// The bus models a group of connections between ICs
// Voltage is modelled rather than logic so analog connections can be simulated

class Bus
{
public:
	Bus(uint16_t size);
	~Bus();

	float Get(uint16_t line)
	{
		return values[line];
	}

	void Set(uint16_t line, float value)
	{	
		values[line] = value;
	}

private:
	std::vector<float> values;
};

