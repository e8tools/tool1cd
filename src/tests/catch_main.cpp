#define CATCH_CONFIG_MAIN
#include <System.SysUtils.hpp>
#include <String.hpp>
#include <MessageRegistration.h>
#include "catch.hpp"

using namespace System;

Registrator msreg_g;
TMultiReadExclusiveWriteSynchronizer* tr_syn = new TMultiReadExclusiveWriteSynchronizer();

TEST_CASE("File extension extracts correctly", "[SysUtils]")
{
	String fileName("/some/file.name");
	String ext = SysUtils::ExtractFileExt(fileName);

	REQUIRE( ext.Compare(".name") == 0 );
}
