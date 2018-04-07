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
