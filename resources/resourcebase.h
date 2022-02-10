// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <vector>

class ResourcePayload
{
public:
	ResourcePayload(){}
	ResourcePayload(const std::string& _name, unsigned int _offset, unsigned int _size)
	: name(_name)
	, offset(_offset)
	, size(_size)
	{}	
	~ResourcePayload(){}

	std::string name;
	unsigned int offset;
	unsigned int size;
};

class ResourceBase
{
public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual bool InitFromFilename(const std::string& filename) = 0;
	const std::vector<ResourcePayload>& GetPayloads() const { return payloads; }
	const char* GetPayload(const std::string& name, std::streamsize& dataSize);

protected:
	char* 	pData;
	std::streamsize	dataSize;
	std::vector<ResourcePayload> payloads;
};

#define IRESOURCEBASE_API \
	virtual bool InitFromFilename(const std::string& filename);
