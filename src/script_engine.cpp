#include "script_engine.h"

template<>
std::string wv::ScriptEngine::popStack() {
	if( !isTopType( LUA_TSTRING ) ) return "";

	std::string v = std::string( lua_tostring( L, -1 ) ); _pop();
	return v;
}

template<>
int wv::ScriptEngine::popStack() {
	if( !isTopType( LUA_TNUMBER ) ) return 0;

	lua_Integer v = lua_tointeger( L, -1 ); _pop();
	return static_cast<int>( v );
}

template<>
float wv::ScriptEngine::popStack() {
	if( !isTopType( LUA_TNUMBER ) ) return 0.0f;

	lua_Number v = lua_tonumber( L, -1 ); _pop();
	return static_cast<float>( v );
}

template<>
double wv::ScriptEngine::popStack() {
	if( !isTopType( LUA_TNUMBER ) ) return 0.0;

	lua_Number v = lua_tonumber( L, -1 ); _pop();
	return static_cast<double>( v );
}

template<>
bool wv::ScriptEngine::popStack() {
	if( !isTopType( LUA_TBOOLEAN ) ) return false;

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

bool wv::ScriptEngine::isTopType( int _type )
{
	if( lua_type( L, 1 ) != _type )
	{
		printf( "Invalid type\n" );
		return false;
	}

	return true;
}


void wv::ScriptEngine::_pop( int _n )
{
	lua_pop( L, _n );
}

