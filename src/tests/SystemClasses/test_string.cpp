#include "../catch.hpp"
#include <String.hpp>

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