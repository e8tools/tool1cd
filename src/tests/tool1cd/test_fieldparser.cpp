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
#include <Field.h>
#include <string>
using namespace std;

static Field *load_field(const string &description)
{
	auto field_tree = parse_1Ctext(description, "");
	Tree *first_entry = field_tree->get_first()->get_first();
	bool has_version = false;
	Field *fld = Field::field_from_tree(first_entry, has_version, nullptr);
	return fld;
}

TEST_CASE( "Проверка разбора полей", "[tool1cd][Fields]" ) {

	GIVEN ("Описание поля binary(16)") {
		string binary16 = "{\"_IDRREF\",\"B\",0,16,0,\"CS\"}";
		WHEN ("Парсим описание поля") {
			Field *fld = load_field(binary16);
			THEN("Имеем верно разобранное поле") {
				REQUIRE(fld->get_name() == "_IDRREF");
				REQUIRE(fld->get_type() == type_fields::tf_binary);
				REQUIRE(fld->get_length() == 16);
				REQUIRE(fld->get_size() == 16);
				REQUIRE(fld->get_null_exists() == false);
			}
		}
	}

	GIVEN ("Описание поля nullable binary(40)") {
		string binary16 = "{\"_DATA\",\"B\",1,40,0,\"CS\"}";
		WHEN ("Парсим описание поля") {
			Field *fld = load_field(binary16);
			THEN("Имеем верно разобранное поле") {
				REQUIRE(fld->get_name() == "_DATA");
				REQUIRE(fld->get_type() == type_fields::tf_binary);
				REQUIRE(fld->get_length() == 40);
				REQUIRE(fld->get_size() == 41);
				REQUIRE(fld->get_null_exists() == true);
			}
		}
	}

	GIVEN ("Описание поля version") {
		string version = "{\"_VERSION\",\"RV\",0,0,0,\"CS\"}";
		WHEN ("Парсим описание поля") {
			Field *fld = load_field(version);
			THEN("Имеем верно разобранное поле") {
				REQUIRE(fld->get_name() == "_VERSION");
				REQUIRE(fld->get_type() == type_fields::tf_version);
				REQUIRE(fld->get_null_exists() == false);
			}
		}
	}
}

TEST_CASE( "Проверка преобразований полей", "[tool1cd][Fields][Types]")
{
	GIVEN ("Описание числового поля с дробной частью") {
		string numeric_field = "{\"AnyName\",\"N\",0,10,4,\"CS\"}";
		Field *fld = load_field(numeric_field);

		char buf[100];

		WHEN ("Выполняем сериализацию/десериализацию строки '10'") {
			string source = "10";
			fld->get_binary_value(buf, false, source);
			string result = fld->get_presentation(buf);

			THEN ("Получаем '10'") {
				REQUIRE (result == "10");
			}
		}

		WHEN ("Выполняем сериализацию/десериализацию строки '10.0'") {
			string source = "10.0";
			fld->get_binary_value(buf, false, source);
			string result = fld->get_presentation(buf);

			THEN ("Получаем '10'") {
				REQUIRE (result == "10");
			}
		}
		WHEN ("Выполняем сериализацию/десериализацию строки '10.90'") {
			string source = "10.90";
			fld->get_binary_value(buf, false, source);
			string result = fld->get_presentation(buf);

			THEN ("Получаем '10.9'") {
				REQUIRE (result == "10.9");
			}
		}
	}
}
