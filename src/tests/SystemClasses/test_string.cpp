#include "../catch.hpp"
#include <String.hpp>

using namespace System;

TEST_CASE("Uppercase в уникоде", "[SystemClasses][String][Unicode]")
{
	GIVEN( "Строка 'Привет'")  {
		String testString("Привет");

		WHEN ("вызываем UpperCase") {
			String result = testString.UpperCase();
			THEN ("получаем 'ПРИВЕТ'") {
				REQUIRE( result.Compare("ПРИВЕТ") == 0 );
			}
		}

		WHEN ("вызываем LowerCase") {
			String result = testString.LowerCase();
			THEN ("получаем 'привет'") {
				REQUIRE( result.Compare("привет") == 0 );
			}
		}
	}
}

TEST_CASE("Upper/Lower Case для каждой буквы", "[SystemClasses][String][Unicode]")
{
	String lowcase("abcdefghijklmnopqrstuvwxyz01234567890"
								"абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
	String upcase("ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890"
						   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");

	REQUIRE(lowcase.UpperCase() == upcase);
	REQUIRE(upcase.LowerCase() == lowcase);

	REQUIRE(lowcase.UpperCase().LowerCase() == lowcase);
	REQUIRE(upcase.LowerCase().UpperCase() == upcase);

	REQUIRE(upcase.UpperCase() == upcase);
	REQUIRE(lowcase.LowerCase() == lowcase);
}