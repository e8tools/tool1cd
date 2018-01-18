#include "../catch.hpp"
#include <Class_1CD.h>
#include <String.hpp>

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
