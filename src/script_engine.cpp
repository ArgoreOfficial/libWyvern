#include "script_engine.h"

template<> bool wv::ScriptEngine::assertType<std::string>( int _stack ) { return lua_isstring( L, _stack ); }
template<> bool wv::ScriptEngine::assertType<float>      ( int _stack ) { return lua_isnumber( L, _stack ); }
template<> bool wv::ScriptEngine::assertType<double>     ( int _stack ) { return lua_isnumber( L, _stack ); }
template<> bool wv::ScriptEngine::assertType<int>        ( int _stack ) { return lua_isnumber( L, _stack ); }
template<> bool wv::ScriptEngine::assertType<bool>       ( int _stack ) { return lua_isboolean( L, _stack ); }

template<>
std::string wv::ScriptEngine::popStack() {
	if( !lua_isstring( L, -1 ) )
	{
		printf( "Expected string, got %s\n", lua_typename( L, -1 ) );
		_pop();
		return {};
	}

	std::string v = std::string( lua_tostring( L, -1 ) ); _pop();
	return v;
}

template<>
int wv::ScriptEngine::popStack() {
	if( !lua_isnumber( L, -1 ) )
	{
		printf( "Expected number, got %s\n", luaL_typename( L, -1 ) );
		_pop();
		return {};
	}

	lua_Integer v = lua_tointeger( L, -1 ); 
	_pop();

	return static_cast<int>( v );
}

template<>
float wv::ScriptEngine::popStack() {
	if( !lua_isnumber( L, -1 ) )
	{
		printf( "Expected number, got %s\n", luaL_typename( L, -1 ) );
		_pop();
		return {};
	}

	lua_Number v = lua_tonumber( L, -1 ); _pop();
	return static_cast<float>( v );
}

template<>
double wv::ScriptEngine::popStack() {
	if( !lua_isnumber( L, -1 ) )
	{
		printf( "Expected number, got %s\n", luaL_typename( L, -1 ) );
		_pop();
		return {};
	}

	lua_Number v = lua_tonumber( L, -1 ); _pop();
	return static_cast<double>( v );
}

template<>
bool wv::ScriptEngine::popStack() {
	if( !lua_isboolean(L, -1) )
	{
		printf( "Expected boolean, got %s\n", luaL_typename( L, -1 ) );
		_pop();
		return {};
	}

	int v = lua_toboolean( L, -1 ); _pop();
	return static_cast<bool>( v );
}

void wv::ScriptEngine::runstring( const std::string& _name, const std::string& _str )
{
	int error = luaL_loadbuffer( L, _str.data(), _str.size(), _name.c_str() ) ||
		lua_pcall( L, 0, 0, 0 );
	assert_err( error );
}

void wv::ScriptEngine::assert_err( int _err )
{
	if( !_err )
		return;

	std::string errstr = popStack<std::string>();
	printf( "%s", errstr.c_str() );
}

void wv::ScriptEngine::_pop( int _n )
{
	lua_pop( L, _n );
}

