// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include <vector>
#include <map>
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
		//std::vector<std::weak_ptr<ResourceInfo>> dependsOn;
		EResourceType type;
		std::string	filename;
		std::string Id;
		std::string windowName;
		//bool updateAvailable;
		//bool needsRefresh;
	};

	bool OpenResourceFromFile(	const std::string& filename, 
								const std::string& Id = "", 
								EResourceType resourceType = EResourceType::Unknown);
	bool CloseResource(const std::string& Id);

	void SetResourceWindow(const std::string &Id, const std::string windowName);

	std::shared_ptr<ResourceBase> GetResourcePtr(const std::string &Id);

	EResourceType ResourceTypeFromString(const std::string& stringDescriptor);
	std::string ResourceTypeToString(EResourceType type);

	const std::map<std::string, ResourceInfo>& GetResources();
	EResourceType GetResourceType(const std::string& resourceId);

private:

//	std::vector<ResourceInfo> resources;
	std::map<std::string, ResourceInfo> resourcesMap;

	std::vector<ResourceTypeInfo> resourceTypeInfo;	// should probably turn into a pair of maps

	bool OpenResource_T64(std::string filename, const std::string& Id);
//	bool OpenResource_D64(std::string filename, const std::string& Id);
};
