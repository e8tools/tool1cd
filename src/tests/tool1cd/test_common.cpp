/*
    test_project provides tests for Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of test_project.

    test_project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    test_project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with test_project.  If not, see <http://www.gnu.org/licenses/>.
*/
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
