// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "resourcebase.h"

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	static ResourceManager* Instance();

	enum class EResourceType
	{
		Unknown,
		T64File,
		D64File,
	};

	struct ResourceTypeInfo
	{
		EResourceType resourceType;
		std::string resourceDescriptorString;
	};

	void OpenResourceFromFile(std::string filename, EResourceType resourceType = EResourceType::Unknown);
	EResourceType ResourceTypeFromString(std::string stringDescriptor);

private:
	std::vector<std::shared_ptr<ResourceBase>> resources;
	std::vector<ResourceTypeInfo> resourceTypeInfo;

	void OpenResource_T64(std::string filename);
	void OpenResource_D64(std::string filename);
};
