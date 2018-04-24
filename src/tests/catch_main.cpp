/*
    test_project provides tests for Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of test_project.

    test_project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    test_project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with test_project.  If not, see <http://www.gnu.org/licenses/>.
*/
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
