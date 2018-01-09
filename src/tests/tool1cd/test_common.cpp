#include "../catch.hpp"
#include <Common.h>
#include <BinaryDecimalNumber.h>
#include <string>

TEST_CASE( "Проверка date_to_string1C", "[tool1cd][Common]" ) {
	char buf[20];
	BinaryDecimalDate bdd(std::string("09.01.2018 12:34:56"));
	bdd.write_to(buf);
	REQUIRE(date_to_string1C(buf) == "20180109123456");
	REQUIRE(date_to_string(buf)   == "09.01.2018 12:34:56");
}
