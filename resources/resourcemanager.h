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

	enum class ResourceType
	{
		EUnknown,
		ED64File
	};

	struct ResourceTypeInfo
	{
		ResourceType resourceType;
		std::string resourceDescriptorString;
	};

	void OpenResourceFromFile(std::string filename, ResourceType resourceType = ResourceType::EUnknown);
	ResourceType ResourceTypeFromString(std::string stringDescriptor);

private:
	std::vector<std::shared_ptr<ResourceBase>> resources;
	std::vector<ResourceTypeInfo> resourceTypeInfo;

	void OpenResource_D64(std::string filename);
};
