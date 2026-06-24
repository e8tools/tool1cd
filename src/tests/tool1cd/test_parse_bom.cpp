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
#include <utility>
#include <SystemClasses/TMemoryStream.hpp>
#include <SystemClasses/System.SysUtils.hpp>

// Прерванное динамическое обновление оставляет в служебных файлах конфигурации
// (например, DynamicallyUpdated) метку порядка байтов UTF-8. Разбор такого
// потока не должен прерываться — BOM пропускается.
TEST_CASE( "Parse_tree пропускает UTF-8 BOM в начале потока", "[tool1cd][common][Parse_tree][BOM]" ) {

	std::string tree_source = "{1,2,{3,4}\n}\n";

	GIVEN ("Поток дерева с UTF-8 BOM (EF BB BF) в начале") {

		auto src = TEncoding::UTF8->fromUtf8(tree_source);
		decltype(src) with_bom;
		with_bom.push_back(0xEF);
		with_bom.push_back(0xBB);
		with_bom.push_back(0xBF);
		with_bom.insert(with_bom.end(), src.begin(), src.end());

		WHEN ("Разбираем поток") {

			TBytesStream bs(std::move(with_bom));
			auto tree = parse_1Cstream(&bs, "");

			THEN ("Дерево разбирается так же, как без BOM") {

				REQUIRE(tree != nullptr);
				std::string out_with_bom;
				tree->outtext(out_with_bom);

				auto reference = parse_1Ctext(tree_source, "");
				REQUIRE(reference != nullptr);
				std::string out_reference;
				reference->outtext(out_reference);

				REQUIRE(out_with_bom == out_reference);
			}
		}
	}
}
