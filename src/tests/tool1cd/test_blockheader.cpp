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
#include "cfapi/APIcfBase.h"
#include <SystemClasses/String.hpp>

TEST_CASE( "Проверка BlockHeader", "[tool1cd][lib][stBlockHeader]" ) {

	GIVEN ("Пустая структура BlockHeader {}") {
		stBlockHeader data = stBlockHeader::create(0, 0, 0);
		WHEN ("Создаём массив байт") {
			std::string bytes = data.toString();
			THEN("Имеем массив '\r\n00000000 00000000 00000000 \r\n'") {

				REQUIRE( System::Equal(bytes, "\r\n00000000 00000000 00000000 \r\n") );
			}
		}
	}
}
