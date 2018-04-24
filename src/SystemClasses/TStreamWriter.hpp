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
#ifndef SYSTEM_TSTREAMWRITER_HPP
#define SYSTEM_TSTREAMWRITER_HPP

#include "TStream.hpp"
#include "System.SysUtils.hpp"
#include "String.hpp"

namespace System {

namespace Classes {

class TStreamWriter
{
public:

	TStreamWriter(const std::string &Filename, TEncoding *Encoding, int BufferSize);
	TStreamWriter(      TStream *stream,   TEncoding* Encoding, int BufferSize);

	void Write(const std::string &s);

private:
	TStream *stream;
};

} // Classes

} // System

#endif
