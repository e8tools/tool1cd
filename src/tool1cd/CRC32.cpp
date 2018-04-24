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
#include "CRC32.h"
#include <boost/crc.hpp>

uint32_t _crc32(TStream* str)
{
	str->Seek(0, soFromBeginning);
	std::streamsize const  buffer_size = 1024;
	boost::crc_32_type result;
	while(true) {
		char  buffer[ buffer_size ];
		auto readBytes = str->Read( &buffer, buffer_size );
		if(!readBytes) {
			break;
		}
		result.process_bytes( buffer, readBytes );
	}
	return result.checksum();
}
