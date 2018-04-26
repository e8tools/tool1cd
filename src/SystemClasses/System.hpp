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

typedef uint32_t         LongWord;
typedef uint16_t         Word;
typedef uint16_t         WORD;
typedef unsigned long    DWORD;
typedef char16_t         WCHART;
typedef int64_t          LONGLONG;
typedef uint64_t         ULONGLONG;
typedef long             LONG;

typedef struct _FILETIME {

	DWORD dwLowDateTime;
	DWORD dwHighDateTime;

} FILETIME;

typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME;


typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG  HighPart;
	};
	struct {
		DWORD LowPart;
		LONG  HighPart;
	} u;
	LONGLONG QuadPart;
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

