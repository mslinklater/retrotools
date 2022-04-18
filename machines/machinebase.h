// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>

class MachineBase
{
public:
	MachineBase();
	virtual ~MachineBase();

	void SysWriteToMemory(unsigned int startAddress, const char* pMemory, unsigned int size);
	
	const std::string& GetName(){ return name; } 
	void SetName(const std::string& _name){ name = _name; }

private:
	std::string name;
};
