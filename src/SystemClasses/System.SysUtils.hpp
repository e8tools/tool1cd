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
#ifndef SYSTEM_SYSUTILS_HPP
#define SYSTEM_SYSUTILS_HPP

#include <vector>

#include "System.IOUtils.hpp"
#include "String.hpp"

namespace System {

namespace SysUtils {

std::string StringReplace(const std::string &S, const std::string &OldPattern, const std::string &NewPattern,
						  int Flags = 0);

class TMultiReadExclusiveWriteSynchronizer
{
public:
	void BeginWrite();
	void EndWrite();

	void BeginRead();
	void EndRead();
};

class TEncoding
{

public:

virtual std::vector<uint8_t> GetPreamble() = 0;
virtual std::string toUtf8(const std::vector<uint8_t> &Buffer, int offset = 0) const = 0;
virtual std::vector<uint8_t> fromUtf8(const std::string &str) = 0;

static int GetBufferEncoding(const std::vector<uint8_t> &Buffer, TEncoding* &AEncoding);
static std::vector<uint8_t> Convert(TEncoding * const Source, TEncoding * const Destination, const std::vector<uint8_t> &Bytes, int StartIndex, int Count);

//! двухбайтная кодировка WCHART
static TEncoding *Unicode;

static TEncoding *UTF8;

};


struct TSearchRec {
	int          Time;
	int64_t      Size;
	int          Attr;
	std::string  Name;
	int          ExcludeAttr;
};

std::string ExtractFileExt(const std::string &filename);

} // SysUtils

namespace Sysutils = SysUtils;

} // System

using namespace System::SysUtils;

#endif
