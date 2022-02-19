// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "resourcemanager.h"
#include "system/log.h"
#include "resource_d64.h"
#include "resource_t64.h"
#include "resource_binary.h"

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
	{
		ResourceTypeInfo info;
		info.resourceType = EResourceType::Binary;
		info.descriptorString = "bin";
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
		// make sure we replace '.' characters with '_'
		std::size_t i;
		while ((i = Id.find('.')) != std::string::npos)
		{
			Id.replace(i, 1, "_");
		}
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
//				OpenResource_D64(filename, Id);
				break;
			case EResourceType::Binary:
				OpenResource_Binary(filename, Id);
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

bool ResourceManager::CloseResource(const std::string& Id)
{
	auto t = resourcesMap.find(Id);
	if(t != resourcesMap.end())
	{
		resourcesMap.erase(Id);
		return true;
	}
	return false;
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

std::shared_ptr<ResourceBase> ResourceManager::GetResourcePtr(const std::string &Id)
{
	auto t = resourcesMap.find(Id);
	if(t != resourcesMap.end())
	{
		return t->second.base;
	}
	return nullptr;
}

void ResourceManager::SetResourceWindow(const std::string &Id, const std::string windowName)
{
	auto t = resourcesMap.find(Id);
	if(t != resourcesMap.end())
	{
		t->second.windowName = windowName;
	}
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

#if 0
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
#endif

bool ResourceManager::OpenResource_Binary(std::string filename, const std::string& Id)
{
	std::shared_ptr<ResourceBinary> newResource(new ResourceBinary);
	newResource->InitFromFilename(filename);

	ResourceInfo info;
	info.Id = Id;
	info.type = EResourceType::Binary;
	info.filename = filename;
	info.base = newResource;

	resourcesMap.emplace(Id, info);
	return true;
}

const std::map<std::string, ResourceManager::ResourceInfo>& ResourceManager::GetResources()
{
	return resourcesMap;
}
