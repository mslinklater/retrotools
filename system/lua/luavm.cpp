// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "luavm.h"

LuaVM::LuaVM()
: pState(nullptr)
{

}

LuaVM::~LuaVM()
{
	if(pState != nullptr)
	{
		lua_close(pState);
		pState = nullptr;
	}
}

eErrorCode LuaVM::ExecuteLine(const std::string& line)
{

	return kError_OK;
}

eErrorCode LuaVM::Init()
{
	pState = lua_newstate( NULL, NULL );

	// load in core systems

	return kError_OK;
}
