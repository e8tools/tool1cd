#include "../catch.hpp"
#include <APIcfBase.h>

TEST_CASE("hex_to_int/int_to_hex получение целого из шестадцатиричного и наоборот", "[tool1cd][Convert][Int]")
{
	std::array<char, HEX_INT_LEN> hex_value = {'0','0','0','0','0','0','0','0'};

	REQUIRE(hex_to_int(hex_value) == 0);
	REQUIRE(int_to_hex(0) == hex_value);

	hex_value = {'0','0','0','0','0','0','1','8'};
	REQUIRE(hex_to_int(hex_value) == 24);
	REQUIRE(int_to_hex(24) == hex_value);

	hex_value = {'0','0','0','0','0','2','0','0'};
	REQUIRE(hex_to_int(hex_value) == 512);
	REQUIRE(int_to_hex(512) == hex_value);

	hex_value = {'0','0','0','0','0','4','0','0'};
	REQUIRE(hex_to_int(hex_value) == 1024);
	REQUIRE(int_to_hex(1024) == hex_value);

}
