#pragma once

#include "lua/lua.hpp"

#include <string>

namespace wv {

class ScriptEngine
{
public:
	ScriptEngine() {
		L = luaL_newstate();
		luaL_openlibs( L );
	}

	~ScriptEngine() {
		if( L )
		{
			lua_close( L );
			L = nullptr;
		}
	}

	template<typename _Ty>
	_Ty popStack();

	template<typename _Ty>
	_Ty getGlobal( const std::string& _name ) {
		if( !lua_getglobal( L, _name.c_str() ) )
		{
			printf( "identifier '%s' not found\n", _name.c_str() );
			return {};
		}
		return popStack<_Ty>();
	}

	template<typename _Ty>
	bool assertType( int _stack = -1 );

	void runstring( const std::string& _name, const std::string& _str );
	void assert_err( int _err );
	
private:
	void _pop( int _n = 1 );

	lua_State* L = nullptr;
};

}
