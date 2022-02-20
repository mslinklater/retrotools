// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

extern "C" {
#include "3rdparty/lua-5.4.4/src/lua.h"
}

class LuaVM
{
public:
	LuaVM();
	~LuaVM();
private:
	lua_State *pState;
};
