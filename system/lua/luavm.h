// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <string>
#include "system/errorcodes.h"

extern "C" {
#include "3rdparty/lua-5.4.4/src/lua.h"
#include "3rdparty/lua-5.4.4/src/lauxlib.h"
#include "3rdparty/lua-5.4.4/src/lualib.h"
}

class LuaVM
{
public:
	LuaVM();
	~LuaVM();

	typedef int(*LuaCallableCFunction)(lua_State*);

	eErrorCode Init();

	eErrorCode ExecuteLine(const std::string& line);

	void RegisterCFunction( LuaCallableCFunction func, const std::string& name );
	void RemoveCFunction( const std::string& name );

	static void DumpStack(lua_State* pState);

private:
	lua_State* pState;
};
