// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "luavm.h"
#include "system/common.h"
#include "utils/stringutils.h"
#include <cassert>
#include <sstream>
#include <iostream>

std::vector<std::string> LuaVM::printOutputBuffer;

static int lua_Print( lua_State* pState )
{
	LUA_FUNCDEF("print");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);

	std::string output(lua_tostring(pState, -1));
	LuaVM::printOutputBuffer.push_back(output);

	while(LuaVM::printOutputBuffer.size() > 100)
	{
		LuaVM::printOutputBuffer.erase(LuaVM::printOutputBuffer.begin());
	}
	return 0;
}

static int lua_LogInfo( lua_State* pState )
{
	LUA_FUNCDEF("loginfo");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);

	std::string output(lua_tostring(pState, -1));
	LOGINFOF("Lua::%s", output.c_str());
	return 0;
}

static int lua_LogWarning( lua_State* pState )
{
	LUA_FUNCDEF("logwarning");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);

	std::string output(lua_tostring(pState, -1));
	LOGWARNINGF("Lua::%s", output.c_str());
	return 0;
}

static int lua_LogError( lua_State* pState )
{
	LUA_FUNCDEF("logerror");
	LUA_ASSERT_NUMPARAMS(1);
	LUA_ASSERT_TYPE(1, LUA_TSTRING);

	std::string output(lua_tostring(pState, -1));
	LOGERRORF("Lua::%s", output.c_str());
	return 0;
}

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
	int ret = luaL_loadbuffer(pState, line.c_str(), line.size(), "Injected");
	
	switch (ret) {
		case LUA_ERRSYNTAX:
			LOGERROR( std::string("Syntax error on load of Lua line: ") + line);
			return kError_Lua;
			break;			
		case LUA_ERRMEM:
			LOGERROR( std::string("Memory error on load of Lua line: ") + line);
			return kError_Lua;
			break;
		default:
			break;
	}
	
	ret = lua_pcall(pState, 0, 0, 0);
	
	switch (ret) {
		case LUA_ERRRUN:
			DumpStack(pState);
			LOGERROR( std::string("Runtime error in Lua line: ") + line);
			return kError_Lua;
			break;
		case LUA_ERRMEM:
			LOGERROR( std::string("Memory error in Lua line: ") + line);
			return kError_Lua;
			break;
		case LUA_ERRERR:
			LOGERROR( std::string("Error while running error handling function in Lua line: ") + line);
			return kError_Lua;
			break;
		default:
			break;
	}	

	return kError_OK;
}

static int panic (lua_State* _state) {
	(void)_state;  /* to avoid warnings */
	const char* pString = lua_tostring(_state, -1);
	LuaVM::DumpStack(_state);
	LOGERROR( std::string("Lua::PANIC: unprotected error in call to Lua API: ") + pString );
	return 0;
}

void LuaHookCall(lua_State* pState, lua_Debug* ar)
{
}

void LuaHookRet(lua_State* pState, lua_Debug* ar)
{
}

void LuaHookLine(lua_State* pState, lua_Debug* ar)
{
}

void LuaHookCount(lua_State* pState, lua_Debug* ar)
{
}

eErrorCode LuaVM::LoadScript(const std::string& filename)
{
	int ret = luaL_loadfile(pState, filename.c_str());

	switch (ret) {
		case LUA_ERRSYNTAX:
			DumpStack(pState);
			LOGERROR( std::string("Lua::Syntax error on load of Lua file: ") + filename);
			return kError_Lua;
			break;			
		case LUA_ERRMEM:
			DumpStack(pState);
			LOGERROR( std::string("Lua::Memory error on load of Lua file: ") + filename);
			return kError_Lua;
			break;			
		case LUA_ERRFILE:
			DumpStack(pState);
			LOGERROR( std::string("Lua::File error on load of Lua file: ") + filename);
			return kError_Lua;
			break;			
		case LUA_ERRERR:
			DumpStack(pState);
			LOGERROR( std::string("Lua::Error on load of Lua file: ") + filename);
			return kError_Lua;
			break;			
		case LUA_ERRRUN:
			DumpStack(pState);
			LOGERROR( std::string("Lua::Run error on load of Lua file: ") + filename);
			return kError_Lua;
			break;			
		default:
			break;
	}

	LOGINFOF("Lua::Loaded file %s OK", filename.c_str());

	ret = lua_pcall(pState, 0, 0, 0);
	
	switch (ret) {
		case LUA_ERRRUN:
			DumpStack(pState);
			LOGERRORF( "Lua::Runtime error: %s", lua_tostring(pState, -1));
			return kError_Lua;
			break;
		case LUA_ERRMEM:
			LOGERRORF( "Lua::Memory error: %s", lua_tostring(pState, -1));
			return kError_Lua;
			break;
		case LUA_ERRERR:
			LOGERRORF( "Lua::Error while running error handling function: ", lua_tostring(pState, -1));
			return kError_Lua;
			break;			
		default:
			break;
	}

	LOGINFOF("Lua::Executed file %s OK", filename.c_str());

	return kError_OK;
}

static void* LuaAlloc( void* ud, void* ptr, size_t osize, size_t nsize )
{
	if (nsize) 
	{
		if (osize) 
		{
			return realloc(ptr, nsize);
		} 
		else 
		{
			return malloc(nsize);
		}
	} 
	else 
	{
		if (osize) 
		{
			free(ptr);
		}
		return nullptr;
	}
}

eErrorCode LuaVM::Init()
{
	pState = lua_newstate( LuaAlloc, NULL );

	assert(pState != nullptr);

	lua_atpanic(pState, &panic);

	// add standard libraries
	luaL_openlibs(pState);

	RegisterCFunction(lua_Print, "myprint");
	RegisterCFunction(lua_LogInfo, "loginfo");
	RegisterCFunction(lua_LogWarning, "logwarning");
	RegisterCFunction(lua_LogError, "logerror");

#if defined(DEBUG)
	lua_sethook(pState, LuaHookCall, LUA_MASKCALL, 0);
	lua_sethook(pState, LuaHookRet, LUA_MASKRET, 0);
	lua_sethook(pState, LuaHookLine, LUA_MASKLINE, 0);
	lua_sethook(pState, LuaHookCount, LUA_MASKCOUNT, 1);
#endif

	// load in core systems
	LoadScript("../lua/init.lua");

	return kError_OK;
}

void LuaVM::DumpStack(lua_State* pState)
{

}

void LuaVM::RegisterCFunction( LuaCallableCFunction func, const std::string& name )
{
	std::vector<std::string> components;

	Stringutils::SplitByString(name, ",", components);

	uint32_t numComponents = components.size();
	
	for (uint32_t i = 0; i < numComponents - 1; i++) {
		if (i == 0) {
			lua_getglobal(pState, components[i].c_str());
		} else {
			lua_getfield(pState, -1, components[i].c_str());
		}
	}
	
	lua_pushcfunction(pState, func);
	
	if (numComponents == 1) {
		lua_setglobal(pState, components[numComponents - 1].c_str());
	} else {
		lua_setfield(pState, -2, components[numComponents - 1].c_str());
	}
	
	lua_pop(pState, (int)(numComponents - 1));	
}

void LuaVM::RemoveCFunction( const std::string& name )
{

}
