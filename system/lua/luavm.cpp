// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "luavm.h"
#include "system/common.h"
#include <cassert>
#include <sstream>

#if 0
static int lua_LoadScript( lua_State* _state )
{
//	FC_LUA_FUNCDEF("FCLoadScript()");
//	FC_LUA_ASSERT_NUMPARAMS(1);
//	FC_LUA_ASSERT_TYPE(1, LUA_TSTRING);

	std::string _desc = "LoadScript()";

	// check there is one argument (the filename) is on the stack
	if( lua_gettop( _state ) != 1 )
	{
		// report error
		std::stringstream error;
		lua_Debug ar;
		lua_getstack(_state, 1, &ar);
		lua_getinfo(_state, "nSl", &ar);
		error << "ERROR: Lua (" << ar.short_src << ":" << ar.currentline << "-" << _desc << "): Wrong number of parameters. Expected " << 1 << " but received " << lua_gettop( _state );
		LOGERROR(error.str());
		LuaVM::DumpStack(_state);
	}

//	common_LoadScriptForState(lua_tostring(_state, -1), _state, false);
	return 0;
}
#endif

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
	LuaVM::DumpStack(_state);
	LOGERROR( std::string("PANIC: unprotected error in call to Lua API: ") + pString );
	return 0;
}

eErrorCode LuaVM::Init()
{
	pState = lua_newstate( NULL, NULL );

	assert(pState != nullptr);

	lua_atpanic(pState, &panic);

	// add standard libraries
	luaL_openlibs(pState);

	// load in core systems

	return kError_OK;
}

void LuaVM::DumpStack(lua_State* pState)
{

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
