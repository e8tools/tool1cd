/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "TempStream.h"
#include <boost/filesystem.hpp>
//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

using namespace std;

boost::filesystem::path TTempStream::tempcat;
bool TTempStream::cleanup = true;
TTempStreamStaticInit TempStreamStaticInit;

static void DelayedInitTempPath()
{
	TTempStream::tempcat = boost::filesystem::temp_directory_path() / "t1cd";
	if (boost::filesystem::exists(TTempStream::tempcat)) {
		boost::filesystem::remove_all(TTempStream::tempcat);
	}
	boost::filesystem::create_directory(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
TTempStreamStaticInit::TTempStreamStaticInit()
{
	if (!TTempStream::tempcat.empty()) {
		return;
	}
}

//---------------------------------------------------------------------------
TTempStreamStaticInit::~TTempStreamStaticInit()
{
	if (!TTempStream::cleanup) {
		return;
	}
	try {
		boost::filesystem::remove_all(TTempStream::tempcat);
	}
	catch (...) {

	}
}

//---------------------------------------------------------------------------
TTempStream::TTempStream() : TFileStream(get_temp_name(), fmCreate)
{
}

//---------------------------------------------------------------------------
TTempStream::~TTempStream()
{
}

//---------------------------------------------------------------------------

std::string TTempStream::get_temp_name()
{
	if (tempcat.empty()) {
		DelayedInitTempPath();
	}
	return (tempcat / boost::filesystem::unique_path()).string();
}

//---------------------------------------------------------------------------

