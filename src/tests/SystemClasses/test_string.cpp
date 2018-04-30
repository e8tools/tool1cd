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
#include <SystemClasses/String.hpp>
#include <SystemClasses/System.SysUtils.hpp>

using namespace System;
using namespace std;

TEST_CASE("Uppercase в уникоде", "[SystemClasses][String][Unicode]")
{
	GIVEN( "Строка 'Привет'")  {
		string testString("Привет");

		WHEN ("вызываем UpperCase") {
			string result = UpperCase(testString);
			THEN ("получаем 'ПРИВЕТ'") {
				REQUIRE( Equal(result, "ПРИВЕТ") );
			}
		}

		WHEN ("вызываем LowerCase") {
			string result = LowerCase(testString);
			THEN ("получаем 'привет'") {
				REQUIRE( Equal(result, "привет") );
			}
		}
	}
}

TEST_CASE("Upper/Lower Case для каждой буквы", "[SystemClasses][String][Unicode]")
{
	string lowcase("abcdefghijklmnopqrstuvwxyz01234567890"
								"абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
	string upcase("ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890"
						   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");

	REQUIRE(UpperCase(lowcase) == upcase);
	REQUIRE(LowerCase(upcase) == lowcase);

	REQUIRE(LowerCase(UpperCase(lowcase)) == lowcase);
	REQUIRE(UpperCase(LowerCase(upcase)) == upcase);

	REQUIRE(UpperCase(upcase) == upcase);
	REQUIRE(LowerCase(lowcase) == lowcase);
}

TEST_CASE("StringReplace", "[SystemClasses][String][Replace]")
{
	REQUIRE(StringReplace("aaa", "a", "ba") == "baaa");
	REQUIRE(StringReplace("aaa", "a", "ba", rfReplaceAll) == "bababa");
}

TEST_CASE("Конвертация строк", "[SystemClasses][String][Encoding]")
{
	GIVEN( "Пустой вектор" ) {
		vector<uint8_t> empty;
		WHEN ("Вызываем UTF8->toUtf8()" ) {
			string result = TEncoding::UTF8->toUtf8(empty);
			THEN ( "Получаем пустую строку" ) {
				REQUIRE (result.empty());
			}
		}
	}
}
