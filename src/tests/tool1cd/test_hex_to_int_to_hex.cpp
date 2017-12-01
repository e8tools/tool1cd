#include "../catch.hpp"
#include <APIcfBase.h>

TEST_CASE("hex_to_int/int_to_hex получение целого из шестадцатиричного и наоборот", "[tool1cd][Convert][Int]")
{
	std::string hex_value = "00000000";

	REQUIRE(hex_to_int(hex_value) == 0);
	REQUIRE(int_to_hex(0) == hex_value);

	hex_value = "00000018";
	REQUIRE(hex_to_int(hex_value) == 24);
	REQUIRE(int_to_hex(24) == hex_value);

	hex_value = "00000200";
	REQUIRE(hex_to_int(hex_value) == 512);
	REQUIRE(int_to_hex(512) == hex_value);

	hex_value = "00000400";
	REQUIRE(hex_to_int(hex_value) == 1024);
	REQUIRE(int_to_hex(1024) == hex_value);

	hex_value = "ffffffe8";
	REQUIRE(hex_to_int(hex_value) == -24);
	REQUIRE(int_to_hex(-24) == hex_value);

}
