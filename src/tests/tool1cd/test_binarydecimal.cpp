#include "../catch.hpp"
#include "BinaryDecimalNumber.h"
#include <System.Classes.hpp>

TEST_CASE( "Проверка обработки Двоично-Десятичных чисел", "[tool1cd][lib][BinaryDecimal]" ) {

	GIVEN ("Строка '12345'") {
		String data = "12345";
		WHEN ("Создаём Двоично-Десятичное число") {
			BinaryDecimalNumber bdn(data);
			THEN("Имеем массив {1, 2, 3, 4, 5}") {
				REQUIRE(bdn.data[0] == 1);
				REQUIRE(bdn.data[1] == 2);
				REQUIRE(bdn.data[2] == 3);
				REQUIRE(bdn.data[3] == 4);
				REQUIRE(bdn.data[4] == 5);
			}

			AND_WHEN ("Получаем представление полученного числа") {
				String presentation = bdn.get_presentation();
				THEN ("Получаем '12345'") {
					REQUIRE(presentation == data);
				}
			}
		}
	}
}