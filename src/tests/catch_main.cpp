#define CATCH_CONFIG_RUNNER
#include <System.SysUtils.hpp>
#include <string>
#include <MessageRegistration.h>
#include <Messenger.h>
#include "catch.hpp"

using namespace System;
using namespace std;

Registrator msreg_g;
TMultiReadExclusiveWriteSynchronizer* tr_syn = new TMultiReadExclusiveWriteSynchronizer();

int main(int argc, char **argv)
{
	Messenger console_registator;
	console_registator.set_log_level(MessageState::Warning);
	msreg_g.AddMessageRegistrator(&console_registator);
	int result = Catch::Session().run(argc, argv);
	return result;
}

TEST_CASE("File extension extracts correctly", "[SysUtils]")
{
	string fileName("/some/file.name");
	string ext = SysUtils::ExtractFileExt(fileName);

	REQUIRE( Compare(ext, ".name") == 0 );
}
