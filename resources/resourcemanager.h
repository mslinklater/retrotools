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

	// mapping from resource type enum to readable string
	struct ResourceTypeInfo
	{
		ResourceBase::EResourceType resourceType;
		std::string descriptorString;
	};

	void OpenResourceFromFile(std::string filename, ResourceBase::EResourceType resourceType = ResourceBase::EResourceType::Unknown);
	
	ResourceBase::EResourceType ResourceTypeFromString(std::string stringDescriptor);
	std::string ResourceTypeToString(ResourceBase::EResourceType type);

	const std::vector<std::shared_ptr<ResourceBase>>& GetResources();

private:
	std::vector<std::shared_ptr<ResourceBase>> resources;

	std::vector<ResourceTypeInfo> resourceTypeInfo;	// should probably turn into a pair of maps

	void OpenResource_T64(std::string filename);
	void OpenResource_D64(std::string filename);
};
