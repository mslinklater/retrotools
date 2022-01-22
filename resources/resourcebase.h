// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>

class ResourceBase
{
public:
	ResourceBase();
	virtual ~ResourceBase();


	virtual bool InitFromFilename(const std::string& filename);

//	EResourceType GetResourceType() { return resourceType; }
//	const std::string& GetFilename() { return filename; }
//	const std::string& GetId(){ return Id; }

protected:
//	EResourceType resourceType;

//	std::string filename;
//	std::string Id;
};
