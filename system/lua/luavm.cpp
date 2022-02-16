// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "luavm.h"
#include "system/common.h"
#include <cassert>

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

static int panic (lua_State* _state) {
	(void)_state;  /* to avoid warnings */
	const char* pString = lua_tostring(_state, -1);
//	FCLua_DumpStack(_state);
	LOGERROR( std::string("PANIC: unprotected error in call to Lua API: ") + pString );
	return 0;
}

eErrorCode LuaVM::Init()
{
	pState = lua_newstate( NULL, NULL );

	assert(pState != nullptr);

	lua_atpanic(pState, &panic);

	// load in core systems

	return kError_OK;
}

void LuaVM::RegisterCFunction( LuaCallableCFunction func, const std::string& name )
{
#if 0
	FCStringVector	components = FCStringUtils_ComponentsSeparatedByString( name, "." );
	
	uint32_t numComponents = components.size();
	
	for (uint32_t i = 0; i < numComponents - 1; i++) {
		if (i == 0) {
			lua_getglobal(m_state, components[i].c_str());
		} else {
			lua_getfield(m_state, -1, components[i].c_str());
		}
	}
	
	lua_pushcfunction(m_state, func);
	
	if (numComponents == 1) {
		lua_setglobal(m_state, components[numComponents - 1].c_str());
	} else {
		lua_setfield(m_state, -2, components[numComponents - 1].c_str());
	}
	
	lua_pop(m_state, (int)(numComponents - 1));	
#endif
}

void LuaVM::RemoveCFunction( const std::string& name )
{

}
