#if defined(DEBUG)

#define LUA_FUNCDEF( n ) std::string _desc = n

#define LUA_ASSERT_TYPE( stackpos, type )	\
{							\
	if( lua_type( pState, stackpos ) != type )	\
	{	\
		std::stringstream error;	\
		lua_Debug ar;	\
		lua_getstack(pState, 1, &ar);	\
		lua_getinfo(pState, "nSl", &ar);	\
		error << "ERROR: Lua (" << ar.short_src << ":" << ar.currentline << "-" << _desc << "): Wrong type, wanted " << lua_typename( pState, type) << ", but found " << lua_typename( pState, lua_type( pState, stackpos));	\
		LOGERROR(error.str());	\
		LuaVM::DumpStack( pState );	\
	}	\
}

#define LUA_ASSERT_NUMPARAMS( n )	\
{										\
	if( lua_gettop( pState ) != n )		\
	{									\
		std::stringstream error;	\
		lua_Debug ar;	\
		lua_getstack(pState, 1, &ar);	\
		lua_getinfo(pState, "nSl", &ar);	\
		error << "ERROR: Lua (" << ar.short_src << ":" << ar.currentline << "-" << _desc << "): Wrong number of parameters. Expected " << n << " but received " << lua_gettop( pState );	\
		LOGERROR(error.str());	\
		LuaVM::DumpStack( pState );		\
	}			\
}

#define LUA_ASSERT_MINPARAMS( n )	\
{										\
	if( lua_gettop( pState ) < n )		\
	{									\
		std::stringstream error;	\
		lua_Debug ar;	\
		lua_getstack(pState, 1, &ar);	\
		lua_getinfo(pState, "nSl", &ar);	\
		error << "ERROR: Lua (" << ar.short_src << ":" << ar.currentline << "-" << _desc << "): Wrong number of parameters. Expected at least " << n << " but received " << lua_gettop( _state );	\
		LOGERROR(error.str());	\
		LuaVM::DumpStack( _state );		\
	}			\
}

#else

#define LUA_FUNCDEF( n ){}
#define LUA_ASSERT_TYPE(stackpos, type){}
#define LUA_ASSERT_NUMPARAMS( n ){}
#define LUA_ASSERT_MINPARAMS( n ){}

#endif	// DEBUG
