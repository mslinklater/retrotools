#pragma once

#include <cstdint>
#include <vector>

class Bus
{
public:
	Bus(uint16_t size);
	virtual ~Bus();

	bool Get(uint16_t line)
	{
		return values[line];
	}

	void Set(uint16_t line, bool value)
	{	
		values[line] = value;
	}

private:
	std::vector<bool> values;
};
