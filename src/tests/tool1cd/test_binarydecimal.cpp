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
#include "BinaryDecimalNumber.h"
#include <System.Classes.hpp>

TEST_CASE( "Проверка обработки Двоично-Десятичных чисел", "[tool1cd][lib][BinaryDecimal]" ) {

	GIVEN ("Строка '12345'") {
		std::string data = "12345";
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
				std::string presentation = bdn.get_presentation();
				THEN ("Получаем '12345'") {
					REQUIRE(presentation == data);
				}
			}
		}
	}
}