// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <memory>
#include "system/window/windowbase.h"

class ResourceBase;

class ResourceWindowBase : public WindowBase
{
public:
	std::shared_ptr<ResourceBase> pResource;
};
