#include "../catch.hpp"
#include <Class_1CD.h>

TEST_CASE("Работа с хранилищем версии 6", "[tool1cd][Class_1CD][depotv6]")
{
	GIVEN( "Хранилище tests/depotv6/depot/1cv8ddb.1CD")  {
		std::string path(CMAKE_SOURCE_DIR);
		path += "/tests/depotv6/depot/1cv8ddb.1CD";

		T_1CD base1CD(path, nullptr, true);
		WHEN ("вызываем is_open") {
			THEN ("получаем 'true'") {
				REQUIRE( base1CD.is_open() == true );
			}
		}
	}
}
