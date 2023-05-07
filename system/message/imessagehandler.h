// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include <memory>

class MessageBase
{
public:
	MessageBase(std::string _name)
	: name (_name )
	{}
	virtual ~MessageBase(){}
	std::string	name;
};

class IMessageHandler
{
public:
	enum Return {
		kForward,
		kConsumed
	};

	virtual Return HandleMessage(const std::shared_ptr<MessageBase> message) = 0;
};

#define IMESSAGEHANDLER_API virtual IMessageHandler::Return HandleMessage(const std::shared_ptr<MessageBase> message) override;
