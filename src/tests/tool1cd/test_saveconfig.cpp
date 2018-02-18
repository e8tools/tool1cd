#include "../catch.hpp"
#include <Class_1CD.h>
#include <boost/filesystem.hpp>

using boost::filesystem::path;

TEST_CASE("Сохранение конфигураций в 8.3.8", "[tool1cd][Class_1CD][cf][8.3.8]")
{
	GIVEN( "База tests/db838/db01/1Cv8.1CD")  {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/db838/db01/1Cv8.1CD";

		T_1CD base1CD(dbpath, nullptr, true);

		WHEN( "Выгружаем конфигурацию" ) {
			path cfpath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
			base1CD.save_configsave(cfpath);
			THEN( "Получаем файл" ) {
				REQUIRE(boost::filesystem::exists(cfpath));
			}
			boost::filesystem::remove(cfpath);
		}
		WHEN( "Выгружаем конфигурацию базы данных" ) {
			path cfpath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
			base1CD.save_config(cfpath);
			THEN( "Получаем файл" ) {
				REQUIRE(boost::filesystem::exists(cfpath));
			}
			boost::filesystem::remove(cfpath);
		}

	}

	GIVEN("База tests/db838/db02/1Cv8.1CD") {
		std::string dbpath(CMAKE_SOURCE_DIR);
		dbpath += "/tests/db838/db02/1Cv8.1CD";

		T_1CD base1CD(dbpath, nullptr, true);
		
		WHEN("Выгружаем конфигурации поставщиков") {
			for (auto& supplier_config : base1CD.supplier_configs()) {
				path cfpath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
				supplier_config->save_to_file(cfpath);

				std::string then_result = std::string( "Получаем файл для " + supplier_config->name() );
				THEN( then_result ) {
					REQUIRE(boost::filesystem::exists(cfpath));
				}
				boost::filesystem::remove(cfpath);
			}
		}

	}
}
