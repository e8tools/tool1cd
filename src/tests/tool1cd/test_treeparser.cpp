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
#include <Parse_tree.h>
#include <string>

TEST_CASE( "Проверка Parse_tree", "[tool1cd][common][Parse_tree]" ) {

	std::string test_tree_source = "{1,2,{3,4}\n}\n";

	GIVEN ("Тестовое дерево") {

		WHEN ("Парсим и выводим дерево через поток") {
			std::string from_stream;
			TBytesStream bs(TEncoding::UTF8->fromUtf8(test_tree_source));
			auto tree_from_stream = parse_1Cstream(&bs, "");
			REQUIRE(tree_from_stream != nullptr);
			tree_from_stream->outtext(from_stream);

			AND_WHEN("Парсим и выводим дерево через строку") {
				std::string from_string;
				auto tree_from_string = parse_1Ctext(test_tree_source, "");
				REQUIRE(tree_from_string != nullptr);
				tree_from_string->outtext(from_string);

				THEN("Получаем одинаковые значения") {
					REQUIRE(from_stream == from_string);
				}
			}
		}
	}
}
