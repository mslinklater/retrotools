// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "resourcemanager.h"
#include "system/log.h"
#include "resource_d64.h"

ResourceManager::ResourceManager()
{
	// register resource types
	{
		ResourceTypeInfo info;
		info.resourceType = ResourceType::ED64File;
		info.resourceDescriptorString = "d64";
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

void ResourceManager::OpenResourceFromFile(std::string filename, ResourceType resourceType)
{
	if(resourceType == ResourceType::EUnknown)
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

	if(resourceType != ResourceType::EUnknown)
	{
		LOGINFOF("ResourceManager::Opening resource %s", filename.c_str());
		switch(resourceType)
		{
			case ResourceType::ED64File:
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

ResourceManager::ResourceType ResourceManager::ResourceTypeFromString(std::string stringDescriptor)
{
	for(const auto& typeInfo : resourceTypeInfo)
	{
		if(typeInfo.resourceDescriptorString == stringDescriptor)
		{
			return typeInfo.resourceType;
		}
	}
	return ResourceType::EUnknown;
}

void ResourceManager::OpenResource_D64(std::string filename)
{
	std::shared_ptr<ResourceD64> newResource(new ResourceD64);
	newResource->InitFromFilename(filename);
	resources.push_back(newResource);
}
