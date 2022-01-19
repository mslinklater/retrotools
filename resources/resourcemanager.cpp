// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "resourcemanager.h"
#include "system/log.h"
#include "resource_d64.h"
#include "resource_t64.h"

ResourceManager::ResourceManager()
{
	// register resource types
	{
		ResourceTypeInfo info;
		info.resourceType = ResourceBase::EResourceType::D64File;
		info.descriptorString = "d64";
		resourceTypeInfo.push_back(info);
	}
	{
		ResourceTypeInfo info;
		info.resourceType = ResourceBase::EResourceType::T64File;
		info.descriptorString = "t64";
		resourceTypeInfo.push_back(info);
	}
}

ResourceManager::~ResourceManager()
{

}

ResourceManager* ResourceManager::Instance()
{
	static ResourceManager* pInstance = nullptr;

	if(pInstance == nullptr)
	{
		pInstance = new ResourceManager;
	}
	return pInstance;
}

void ResourceManager::OpenResourceFromFile(std::string filename, ResourceBase::EResourceType resourceType)
{
	if(resourceType == ResourceBase::EResourceType::Unknown)
	{
		// work out what sort of resource it is and instantiate a resource of that type

		// does the filename have a suffix which matches a known type ?
		std::size_t dotPos = filename.find('.');
		if(dotPos != std::string::npos)
		{
			std::string suffix = filename.substr(dotPos+1);
			resourceType = ResourceTypeFromString(suffix);
		}
	}

	// now load the resource

	if(resourceType != ResourceBase::EResourceType::Unknown)
	{
		LOGINFOF("ResourceManager::Opening resource %s", filename.c_str());
		switch(resourceType)
		{
			case ResourceBase::EResourceType::T64File:
				OpenResource_T64(filename);
				break;
			case ResourceBase::EResourceType::D64File:
				OpenResource_D64(filename);
				break;
			default:
				LOGERRORF("ResourceManager::OPenResourceFromFile not implemented for known type %s", filename.c_str());
				break;
		}
	}
	else
	{
		LOGERRORF("ResourceManager::Unknown resource type %s", filename.c_str());
	}
}

ResourceBase::EResourceType ResourceManager::ResourceTypeFromString(std::string stringDescriptor)
{
	for(const auto& typeInfo : resourceTypeInfo)
	{
		if(typeInfo.descriptorString == stringDescriptor)
		{
			return typeInfo.resourceType;
		}
	}
	return ResourceBase::EResourceType::Unknown;
}

void ResourceManager::OpenResource_T64(std::string filename)
{
	std::shared_ptr<ResourceT64> newResource(new ResourceT64);
	newResource->InitFromFilename(filename);
	resources.push_back(newResource);
}

void ResourceManager::OpenResource_D64(std::string filename)
{
	std::shared_ptr<ResourceD64> newResource(new ResourceD64);
	newResource->InitFromFilename(filename);
	resources.push_back(newResource);
}
