#include "../catch.hpp"
#include <Class_1CD.h>
#include <boost/filesystem.hpp>
#include <String.hpp>

using boost::filesystem::path;
using namespace System;

TEST_CASE("Работа с хранилищем версии 5", "[tool1cd][Class_1CD][depotv5]")
{
	GIVEN( "Хранилище tests/depotv5/depot/1cv8ddb.1CD")  {
		std::string path(CMAKE_SOURCE_DIR);
		path += "/tests/depotv5/depot/1cv8ddb.1CD";

		T_1CD base1CD(path, nullptr, true);

		WHEN ("вызываем is_open") {
			bool result = base1CD.is_open();
			THEN ("получаем 'true'") {
				REQUIRE( result == true );
			}
		}

		WHEN ("обращаемся к полю ver") {
			String ver = base1CD.ver;
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

		WHEN ("обращаемся к полю is_infobase") {
			bool result = base1CD.is_infobase;
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

TEST_CASE("Сохраняем конфигурацию хранилища 5", "[tool1cd][Class_1CD][depotv5]")
{
	GIVEN( "Хранилище tests/depotv5/depot/1cv8ddb.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/depotv5/depot/1cv8ddb.1CD";

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
