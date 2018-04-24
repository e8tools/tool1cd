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

string TTempStream::tempcat;
long TTempStream::tempno = 0;
TTempStreamStaticInit TempStreamStaticInit;

//---------------------------------------------------------------------------
TTempStreamStaticInit::TTempStreamStaticInit()
{
	if (!TTempStream::tempcat.empty()) {
		return;
	}
	boost::filesystem::path p_tempcat = boost::filesystem::temp_directory_path() / "awa";
	TTempStream::tempcat = p_tempcat.string();
	DeleteFile(TTempStream::tempcat);
	CreateDir(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
TTempStreamStaticInit::~TTempStreamStaticInit()
{
	RemoveDir(TTempStream::tempcat);
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
	return (boost::filesystem::path(tempcat) / boost::filesystem::unique_path()).string();
}

//---------------------------------------------------------------------------

