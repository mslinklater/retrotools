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

	// mapping from resource type enum to readable string
	struct ResourceTypeInfo
	{
		EResourceType resourceType;
		std::string descriptorString;
	};

	struct ResourceInfo
	{
		std::shared_ptr<ResourceBase> base;
		EResourceType type;
		std::string	filename;
		std::string Id;
	};

	void OpenResourceFromFile(	const std::string& filename, 
								const std::string& Id = "", 
								EResourceType resourceType = EResourceType::Unknown);
	
	EResourceType ResourceTypeFromString(const std::string& stringDescriptor);
	std::string ResourceTypeToString(EResourceType type);

	const std::vector<ResourceInfo>& GetResources();

private:
	std::vector<ResourceInfo> resources;

	std::vector<ResourceTypeInfo> resourceTypeInfo;	// should probably turn into a pair of maps

	void OpenResource_T64(std::string filename);
	void OpenResource_D64(std::string filename);
};
