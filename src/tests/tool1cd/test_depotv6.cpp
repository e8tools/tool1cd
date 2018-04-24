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
#include <Class_1CD.h>
#include <boost/filesystem.hpp>
#include <String.hpp>

using boost::filesystem::path;
using namespace System;
using namespace std;

TEST_CASE("Работа с хранилищем версии 6", "[tool1cd][Class_1CD][depotv6]")
{
	GIVEN( "Хранилище tests/depotv6/depot/1cv8ddb.1CD")  {
		std::string path(CMAKE_SOURCE_DIR);
		path += "/tests/depotv6/depot/1cv8ddb.1CD";

		T_1CD base1CD(path, nullptr, true);

		WHEN ("вызываем is_open") {
			bool result = base1CD.is_open();
			THEN ("получаем 'true'") {
				REQUIRE( result == true );
			}
		}

		WHEN ("обращаемся к полю ver") {
			string ver = base1CD.ver;
			THEN ("получаем '8.2.14.0'") {
				REQUIRE( ver == "8.2.14.0" );
			}
		}

		WHEN ("обращаемся к полю is_depot") {
			bool result = base1CD.is_depot;
			THEN ("получаем 'true'") {
				REQUIRE( result == true );
			}
		}

		WHEN ("вызываем is_infobase") {
			bool result = base1CD.is_infobase();
			THEN ("получаем 'false'") {
				REQUIRE( result == false );
			}
		}

		WHEN ("вызываем get_readonly") {
			bool result = base1CD.get_readonly();
			THEN ("получаем 'false'") {
				REQUIRE( result == false );
			}
		}

	}
}

TEST_CASE("Сохраняем конфигурацию хранилища 6", "[tool1cd][Class_1CD][depotv6]")
{
	GIVEN( "Хранилище tests/depotv6/depot/1cv8ddb.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/depotv6/depot/1cv8ddb.1CD";

		path cfpath = boost::filesystem::temp_directory_path() / "tmp.cf";

		T_1CD base1CD(dbpath, nullptr, true);
		WHEN ("Сохраняем конфигурацию") {
			base1CD.save_depot_config(cfpath.string());
			THEN ("получаем выгруженный файл") {
				REQUIRE( boost::filesystem::exists(cfpath) );
			}
		}
	}
}

TEST_CASE("Сохраняем часть конфигурации хранилища 6", "[tool1cd][Class_1CD][depotv6]")
{
	GIVEN( "Хранилище tests/depotv6/depot/1cv8ddb.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/depotv6/depot/1cv8ddb.1CD";

		path cfpath = boost::filesystem::temp_directory_path() / "tmp.d6";

		T_1CD base1CD(dbpath, nullptr, true);
		WHEN ("Сохраняем конфигурацию") {
			base1CD.save_part_depot_config(cfpath.string(), 1, 1);
			THEN ("не падаем") {
				REQUIRE( true );
			}
		}
	}
}
