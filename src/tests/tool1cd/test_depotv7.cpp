#include "../catch.hpp"
#include <Class_1CD.h>
#include <boost/filesystem.hpp>
#include <String.hpp>

using boost::filesystem::path;
using namespace System;
using namespace std;

TEST_CASE("Работа с хранилищем версии 7", "[tool1cd][Class_1CD][depotv7]")
{
	GIVEN( "Хранилище tests/depotv7/depot/1cv8ddb.1CD")  {
		std::string path(CMAKE_SOURCE_DIR);
		path += "/tests/depotv7/depot/1cv8ddb.1CD";

		T_1CD base1CD(path, nullptr, true);

		WHEN ("вызываем is_open") {
			bool result = base1CD.is_open();
			THEN ("получаем 'true'") {
				REQUIRE( result == true );
			}
		}

		WHEN ("обращаемся к полю ver") {
			string ver = base1CD.ver;
			THEN ("получаем '8.3.8.0'") {
				REQUIRE( ver == "8.3.8.0" );
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

TEST_CASE("Сохраняем конфигурацию хранилища 7", "[tool1cd][Class_1CD][depotv7]")
{
	GIVEN( "Хранилище tests/depotv7/depot/1cv8ddb.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/depotv7/depot/1cv8ddb.1CD";

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

TEST_CASE("Сохраняем часть конфигурации хранилища 7", "[tool1cd][Class_1CD][depotv7]")
{
	GIVEN( "Хранилище tests/depotv7/depot/1cv8ddb.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/depotv7/depot/1cv8ddb.1CD";

		path cfpath = boost::filesystem::temp_directory_path() / "tmp.d7";

		T_1CD base1CD(dbpath, nullptr, true);
		WHEN ("Сохраняем конфигурацию") {
			base1CD.save_part_depot_config(cfpath.string(), 1, 1);
			THEN ("не падаем") {
				REQUIRE( true );
			}
		}
	}
}
