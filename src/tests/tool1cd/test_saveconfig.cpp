#include "../catch.hpp"
#include <Class_1CD.h>
#include <boost/filesystem.hpp>

using boost::filesystem::path;

TEST_CASE("Сохранение конфигураций в 8.3.8", "[tool1cd][Class_1CD][cf][8.3.8]")
{
	GIVEN( "Бааза tests/db838/db01/1Cv8.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/db838/db01/1Cv8.1CD";

		T_1CD base1CD(dbpath, nullptr, true);

		path cfpath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();

		WHEN( "Выгружаем конфигурацию базы данных" ) {
			base1CD.save_config(cfpath);
			THEN( "Получаем файл" ) {
				REQUIRE(boost::filesystem::exists(cfpath));
			}
			boost::filesystem::remove(cfpath);
		}

	}
}
