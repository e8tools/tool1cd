#define CATCH_CONFIG_MAIN
#include <System.SysUtils.hpp>
#include <String.hpp>
#include <MessageRegistration.h>
#include "catch.hpp"

using namespace System;

Registrator msreg_g;

TEST_CASE("File extension extracts correctly", "[SysUtils]")
{
	String fileName("/some/file.name");
	String ext = SysUtils::ExtractFileExt(fileName);

	REQUIRE( ext.Compare(".name") == 0 );
}
