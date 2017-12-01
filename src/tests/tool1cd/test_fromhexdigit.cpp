#include "../catch.hpp"
#include <Common.h>

TEST_CASE( "Проверка from_hex_digit", "[tool1cd][from_hex_digit]" ) {
	REQUIRE( from_hex_digit('0') == 0 );
	REQUIRE( from_hex_digit('1') == 1 );
	REQUIRE( from_hex_digit('2') == 2 );
	REQUIRE( from_hex_digit('3') == 3 );
	REQUIRE( from_hex_digit('4') == 4 );
	REQUIRE( from_hex_digit('5') == 5 );
	REQUIRE( from_hex_digit('6') == 6 );
	REQUIRE( from_hex_digit('7') == 7 );
	REQUIRE( from_hex_digit('8') == 8 );
	REQUIRE( from_hex_digit('9') == 9 );

	REQUIRE( from_hex_digit('a') == 10 );
	REQUIRE( from_hex_digit('b') == 11 );
	REQUIRE( from_hex_digit('c') == 12 );
	REQUIRE( from_hex_digit('d') == 13 );
	REQUIRE( from_hex_digit('e') == 14 );
	REQUIRE( from_hex_digit('f') == 15 );

	REQUIRE( from_hex_digit('A') == 10 );
	REQUIRE( from_hex_digit('B') == 11 );
	REQUIRE( from_hex_digit('C') == 12 );
	REQUIRE( from_hex_digit('D') == 13 );
	REQUIRE( from_hex_digit('E') == 14 );
	REQUIRE( from_hex_digit('F') == 15 );

	REQUIRE( from_hex_digit('Y') == 0 );
	REQUIRE( from_hex_digit('y') == 0 );

	REQUIRE( from_hex_digit('#') == 0 );
	REQUIRE( from_hex_digit('$') == 0 );
}
