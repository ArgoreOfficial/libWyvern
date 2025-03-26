#include <stdio.h>

#include "test.hpp"
#include "tests/test_array_view.hpp"
#include "tests/test_ptr_reloc.hpp"
#include "tests/test_reflected_function.hpp"
#include "tests/test_registry.hpp"
#include "tests/test_strong_type.hpp"
#include "tests/test_unordered_array.hpp"

#include "script_engine.h"

#include <stdint.h>

#include <typeinfo>
#include <vector>
#include <array>
#include <string>
#include <atomic>
#include <thread>

#include <stdio.h>
#include <string.h>

#include <wv/type_traits.hpp>
#include "wip/type_descriptor.h"
#ifdef WV_CPP17
struct test_struct
{
	uint16_t member_int;
	float member_float;
	char member_char;

	void dothing2() {}
};

typedef type_layout<
		test_struct,
		&test_struct::member_int,
		&test_struct::member_float,
		&test_struct::member_char,
		&test_struct::dothing2> 
	test_struct_layout;
#define test_struct_member_names { "member_int", "member_float", "member_char", "dothing2" }
#endif

#include <array>
#include <unordered_map>

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


namespace wv {

size_t constexpr StringLength( const char* str ) { 
	return *str ? 1 + StringLength( str + 1 ) : 0;
}

template <size_t _Len>
struct ConstString
{
	char data[ _Len ] = { 0 };
	
	constexpr ConstString( const char( &_cstr )[ _Len ] ) noexcept {
		for( size_t i = 0; i < _Len; ++i )
			data[ i ] = *( _cstr + i );
	}

	template<size_t _Len2>
	constexpr ConstString( const ConstString<_Len2> _other, const size_t _offset = 0, const size_t _len = _Len ) noexcept {
		for( size_t i = 0; i < _len; ++i )
			data[ i ] = *( _other.c_str() + _offset + i );
	}

	constexpr const char* c_str() const noexcept { return &data[ 0 ]; }
	constexpr std::size_t size() const noexcept { return _Len - 1; }
};

template<typename ... _Tys>
static inline constexpr auto ConstTypeName() { return ConstString<StringLength( __PRETTY_FUNCTION__ ) + 1>( __PRETTY_FUNCTION__ ); }

size_t constexpr TypeStringOffset( const char* _a, const char* _b ) {
	return (*_a) == (*_b) 
		? 1 + TypeStringOffset( _a + 1, _b + 1)
		: 0;
}

template<typename _Ty>
static constexpr auto TypeString() {
	constexpr auto bname = ConstTypeName();
	constexpr auto tname = ConstTypeName<_Ty>();
	constexpr size_t size   = tname.size() - bname.size();
	constexpr size_t offset = TypeStringOffset( bname.c_str(), tname.c_str() );
	return ConstString<size + 1>( tname, offset, size );
}

}

int main()
{
	wv::test_array_view();
	wv::test_ptr_reloc();
	wv::test_reflected_function();
	wv::test_registry();
	wv::test_strong_type();
	wv::test_unordered_array();

	printf( "%s\n", wv::TypeString<int>().c_str() );
	printf( "%s\n", wv::TypeString<float>().c_str() );
	printf( "%s\n", wv::TypeString<wv::ScriptEngine>().c_str() );

	wv::ScriptEngine luaEngine;

	std::string test =
		"local p = {1,2,3,2,3,2,1}\n"
		"glob = 0\n"
		"globstr = 'asd'\n"
		"\n"
		"for i=1, #p + 1 do\n"
		"  glob = glob + (p[i] and p[i] or 0)\n"
		"end\n"
		"print('glob is', glob)\n";

	printf( "%s\n", test.c_str() );
	printf( "Running:\n" );
	luaEngine.runstring( "test_str", test );
	int glob = luaEngine.getGlobal<int>( "globstr" );
	printf( "Glob: %i\n", glob );
	
#ifdef WV_CPP17
	printf( " ::------ type_layout test ------::\n" );
	test_struct coolthing;

	type_descriptor<test_struct> test_struct_set{ 
		test_struct_layout{ test_struct_member_names } 
	};

	test_struct_set.set( "member_int", 0 );
	printf( " ::------------------------------::\n\n" );
#endif

	return 0;
}
