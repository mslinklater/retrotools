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

	enum class EResourceType
	{
		Unknown,
		T64File,
		D64File,
	};

	virtual void InitFromFilename(std::string filename);
	
protected:

};
