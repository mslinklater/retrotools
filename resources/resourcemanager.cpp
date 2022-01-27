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
		info.resourceType = EResourceType::D64File;
		info.descriptorString = "d64";
		resourceTypeInfo.push_back(info);
	}
	{
		ResourceTypeInfo info;
		info.resourceType = EResourceType::T64File;
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

bool ResourceManager::OpenResourceFromFile(const std::string& filename, const std::string& _Id, EResourceType resourceType)
{
	std::string Id(_Id);

	if(resourceType == EResourceType::Unknown)
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

	// build a valid Id if one not given
	if(Id.size() == 0)
	{
		Id = filename;
	}

	// check for Id clashes and extend the Id to make it unique

	// now load the resource

	if(resourceType != EResourceType::Unknown)
	{
		LOGINFOF("ResourceManager::Opening resource %s", filename.c_str());
		switch(resourceType)
		{
			case EResourceType::T64File:
				OpenResource_T64(filename, Id);
				break;
			case EResourceType::D64File:
				OpenResource_D64(filename, Id);
				break;
			default:
				LOGERRORF("ResourceManager::OpenResourceFromFile not implemented for known type %s", filename.c_str());
				return false;
				break;
		}
	}
	else
	{
		LOGERRORF("ResourceManager::Unknown resource type %s", filename.c_str());
		return false;
	}
	return true;
}

bool ResourceManager::DeleteResource(const std::string& Id)
{
	return true;
}

ResourceManager::EResourceType ResourceManager::GetResourceType(const std::string& resourceId)
{
	auto t = resourcesMap.find(resourceId);
	if(t != resourcesMap.end())
	{
		return t->second.type;
	}
	return EResourceType::Unknown;
}

ResourceManager::EResourceType ResourceManager::ResourceTypeFromString(const std::string& stringDescriptor)
{
	for(const auto& typeInfo : resourceTypeInfo)
	{
		if(typeInfo.descriptorString == stringDescriptor)
		{
			return typeInfo.resourceType;
		}
	}
	return EResourceType::Unknown;
}

std::string ResourceManager::ResourceTypeToString(EResourceType type)
{
	for(const auto& typeInfo : resourceTypeInfo)
	{
		if(typeInfo.resourceType == type)
		{
			return typeInfo.descriptorString;
		}
	}
	return "Unknown";
}

bool ResourceManager::OpenResource_T64(std::string filename, const std::string& Id)
{
	std::shared_ptr<ResourceT64> newResource(new ResourceT64);
	newResource->InitFromFilename(filename);

	ResourceInfo info;
	info.Id = Id;
	info.type = EResourceType::T64File;
	info.filename = filename;
	info.base = newResource;

	resourcesMap.emplace(Id, info);
//	resources.push_back(info);
	return true;
}

bool ResourceManager::OpenResource_D64(std::string filename, const std::string& Id)
{
	std::shared_ptr<ResourceD64> newResource(new ResourceD64);
	newResource->InitFromFilename(filename);

	ResourceInfo info;
	info.Id = Id;
	info.type = EResourceType::D64File;
	info.filename = filename;
	info.base = newResource;

	resourcesMap.emplace(Id, info);
//	resources.push_back(info);
	return true;
}

//const std::vector<ResourceManager::ResourceInfo>& ResourceManager::GetResources()
//{
//	return resources;
//}

const std::map<std::string, ResourceManager::ResourceInfo>& ResourceManager::GetResources()
{
	return resourcesMap;
}
