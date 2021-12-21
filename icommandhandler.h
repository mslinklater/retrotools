// [CopyrightNotice]
// Copyright (c) 2019-2021, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <memory>

class CommandBase
{
public:
	virtual ~CommandBase(){}
	std::string	name;
};

class ICommandHandler
{
public:
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command) = 0;
};
