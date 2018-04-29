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
#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifndef DELPHI_PACKAGE
#define DELPHI_PACKAGE
#endif // DELPHI_PACKAGE

#include <cstdint>
#include <vector>
#include <climits>
#include <ctime>

const uint32_t WINDOWS_TICK = 10000000;
const uint64_t SEC_TO_UNIX_EPOCH = 11644473600LL;

namespace System {


typedef char16_t         WCHART;


typedef struct _FILETIME {

	uint32_t dwLowDateTime;
	uint32_t dwHighDateTime;

} FILETIME;

typedef struct _SYSTEMTIME {
	uint16_t wYear;
	uint16_t wMonth;
	uint16_t wDayOfWeek;
	uint16_t wDay;
	uint16_t wHour;
	uint16_t wMinute;
	uint16_t wSecond;
	uint16_t wMilliseconds;
} SYSTEMTIME;


typedef union _LARGE_INTEGER {
	struct {
		uint32_t LowPart;
		long  HighPart;
	};
	struct {
		uint32_t LowPart;
		long  HighPart;
	} u;
	int64_t QuadPart;
} LARGE_INTEGER;



uint64_t FileTime_to_POSIX(System::FILETIME * pft);
void UnixTimeToFileTime(time_t t, System::FILETIME * pft);

bool SystemTimeToFileTime(const System::SYSTEMTIME *lpSystemTime, System::FILETIME * lpFileTime);
bool LocalFileTimeToFileTime(const System::FILETIME *lpLocalFileTime, System::FILETIME * lpFileTime);
bool FileTimeToLocalFileTime(const System::FILETIME *lpLocalFileTime, System::FILETIME * lpFileTime);

void GetSystemTime(System::SYSTEMTIME *lpSystemTime);

enum TReplaceFlags {
	rfReplaceAll = 1,
	rfIgnoreCase = 2
};

} // System

using System::WCHART;

#include "String.hpp"

#endif

