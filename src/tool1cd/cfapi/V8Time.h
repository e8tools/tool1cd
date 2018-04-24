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
#ifndef V8TIME_H
#define V8TIME_H

#include <System.Classes.hpp>

#ifdef _MSC_VER

	#include <sys/utime.h>

#else

	#include <sys/types.h>
	#include <utime.h>

#endif // _MSC_VER

using namespace System;

class V8Time
{
public:

	static const int64_t EPOCH_START_WIN;
	static V8Time current_time();

	explicit V8Time();
	explicit V8Time(const int64_t value);
	explicit V8Time(const FILETIME &value);

	 ~V8Time() = default;

	FILETIME to_file_time() const;
	void from_file_time(const FILETIME &value);
	size_t write_to_stream(TMemoryStream *out_stream) const;

#ifdef _MSC_VER

	static _utimbuf to_file_times(const V8Time &create, const V8Time &modify);

#else

	static utimbuf to_file_times(const V8Time &create, const V8Time &modify);

#endif // _MSC_VER

private:
	int64_t _data {0};

	int64_t inner_from_file_time(const FILETIME &value);
};


#endif // V8TIME_H
