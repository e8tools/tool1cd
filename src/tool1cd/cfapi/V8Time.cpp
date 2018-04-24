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
#include "V8Time.h"

using namespace System;

//504911232000000 = ((365 * 4 + 1) * 100 - 3) * 4 * 24 * 60 * 60 * 10000
const int64_t V8Time::EPOCH_START_WIN = 504911232000000;

V8Time V8Time::current_time() {
	SYSTEMTIME st;
	FILETIME ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	V8Time result(ft);

	return result;
}

V8Time::V8Time() {

}

V8Time::V8Time(const int64_t value)
	: _data(value)
{

}

V8Time::V8Time(const System::FILETIME &value) {
	_data = inner_from_file_time(value);
}

FILETIME V8Time::to_file_time() const {
	FILETIME local_file_time;
	FILETIME result;

	int64_t tmp = _data;
	tmp -= EPOCH_START_WIN;
	tmp *= 1000;
	*(int64_t*)&local_file_time = tmp;
	LocalFileTimeToFileTime(&local_file_time, &result);

	return result;
}

void V8Time::from_file_time(const FILETIME &value) {
	_data = inner_from_file_time(value);
}

int64_t V8Time::inner_from_file_time(const FILETIME &value) {
	FILETIME local_file_time;

	FileTimeToLocalFileTime(&value, &local_file_time);
	int64_t tmp = *(int64_t*)&local_file_time;
	tmp /= 1000;
	tmp += EPOCH_START_WIN;

	return tmp;
}

size_t V8Time::write_to_stream(TMemoryStream *out_stream) const {
	return out_stream->Write(&_data, sizeof(_data));
}


#ifdef _MSC_VER

_utimbuf V8Time::to_file_times(const V8Time &create, const V8Time &modify) {
	struct _utimbuf ut;

	FILETIME _create = create.to_file_time();
	FILETIME _modify = modify.to_file_time();

	time_t RawtimeCreate = FileTime_to_POSIX(&_create);
	struct tm * ptm_create = localtime(&RawtimeCreate);
	ut.actime = mktime(ptm_create);

	time_t RawtimeModified = FileTime_to_POSIX(&_modify);
	struct tm * ptm_modified = localtime(&RawtimeModified);
	ut.modtime = mktime(ptm_modified);

	return ut;

}

#else

utimbuf V8Time::to_file_times(const V8Time &create, const V8Time &modify) {
	struct utimbuf ut;

	FILETIME _create = create.to_file_time();
	FILETIME _modify = modify.to_file_time();

	time_t RawtimeCreate = FileTime_to_POSIX(&_create);
	struct tm * ptm_create = localtime(&RawtimeCreate);
	ut.actime = mktime(ptm_create);

	time_t RawtimeModified = FileTime_to_POSIX(&_modify);
	struct tm * ptm_modified = localtime(&RawtimeModified);
	ut.modtime = mktime(ptm_modified);

	return ut;
}

#endif // _MSC_VER
