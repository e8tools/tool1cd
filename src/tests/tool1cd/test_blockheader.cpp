#include "../catch.hpp"
#include "cfapi/APIcfBase.h"
#include <System.Classes.hpp>

TEST_CASE( "Проверка BlockHeader", "[tool1cd][lib][stBlockHeader]" ) {

	GIVEN ("Пустая структура BlockHeader {}") {
		stBlockHeader data = stBlockHeader::create(0, 0, 0);
		WHEN ("Создаём массив байт") {
			std::string bytes ( reinterpret_cast<char*>(&data) );
			THEN("Имеем массив '\r\n00000000 00000000 00000000 \r\n'") {
				std::cout << bytes << std::endl;
				REQUIRE( Equal(bytes, "\r\n00000000 00000000 00000000 \r\n") );
			}
		}
	}
}
