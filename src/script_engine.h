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

	void runstring( const std::string& _name, const std::string& _str );
	void assert_err( int _err );
	bool isTopType( int _type );

private:
	void _pop( int _n = 1 );

	lua_State* L = nullptr;
};

}
