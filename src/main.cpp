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

int main()
{
	wv::test_array_view();
	wv::test_ptr_reloc();
	wv::test_reflected_function();
	wv::test_registry();
	wv::test_strong_type();
	wv::test_unordered_array();

	wv::ScriptEngine luaEngine;

	std::string test =
		"local p = {1,2,3,2,3,2,1}\n"
		"\n"
		"for i=1, #p + 1 do\n"
		"  print(p[i] + i)\n"
		"end\n";
	printf( "%s\n", test.c_str() );
	printf( "Running:\n" );
	luaEngine.runstring( "test_str", test );

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
