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
#include "System.hpp"

namespace System {

//---------------------------------------------------------------------------
// Замена макросов Microsoft
int64_t Int32x32To64(int64_t a, int64_t b)
{
	return ((int64_t)(((int64_t)((long)(a))) * ((long)(b))));
}

//---------------------------------------------------------------------------
// Замена макросов Microsoft
uint64_t UInt32x32To64(int64_t a, int64_t b)
{
	return ((uint64_t)(((uint64_t)((unsigned int)(a))) * ((unsigned int)(b))));
}


//---------------------------------------------------------------------------
// Преобразование времени FILETIME to POSIX
uint64_t FileTime_to_POSIX(System::FILETIME * pft)
{
	// takes the last modified date
	LARGE_INTEGER date, adjust;
	date.HighPart = pft -> dwHighDateTime;
	date.LowPart  = pft -> dwLowDateTime;

	// 100-nanoseconds = milliseconds * 10000
	adjust.QuadPart = SEC_TO_UNIX_EPOCH * 10000;

	// removes the diff between 1970 and 1601
	date.QuadPart -= adjust.QuadPart;

	// converts back from 100-nanoseconds to seconds
	return date.QuadPart / 10000000;
}

//---------------------------------------------------------------------------
// Преобразование времени POSIX to FILETIME
void UnixTimeToFileTime(time_t t, System::FILETIME * pft)
{

	uint64_t ll = UInt32x32To64(t, 10000000) + SEC_TO_UNIX_EPOCH;

	pft->dwLowDateTime = (uint32_t)ll;
	pft->dwHighDateTime = ll >> 32;
}


bool SystemTimeToFileTime(const SYSTEMTIME *lpSystemTime, System::FILETIME * lpFileTime)
{
	struct tm ttm;
	time_t UnixTime;

	ttm.tm_year  = lpSystemTime->wYear - 1900;
	ttm.tm_mon   = lpSystemTime->wMonth - 1;
	ttm.tm_mday  = lpSystemTime->wDay;
	ttm.tm_hour  = lpSystemTime->wHour;
	ttm.tm_min   = lpSystemTime->wMinute;
	ttm.tm_sec   = lpSystemTime->wSecond;
	ttm.tm_isdst = 0;
	ttm.tm_wday  = 0; // возможно здесь что-то не то, надо проверять; days since Sunday - [0, 6] 
	ttm.tm_yday  = 0; // возможно здесь что-то не то, надо проверять; days since January 1 - [0, 365]
	
	UnixTime = mktime(&ttm);
	UnixTimeToFileTime(UnixTime, lpFileTime);

	return true; 
}

bool LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime, System::FILETIME * lpFileTime)
{
	
	// TODO: реализовать кроссплатформенное преобразование локального системного файлового времени в универсальное (UTC) файловое время
	time_t rawtime  = FileTime_to_POSIX((System::FILETIME *)lpLocalFileTime);
	struct tm *ptm  = gmtime(&rawtime);
	time_t UnixTime = mktime(ptm);
	UnixTimeToFileTime(UnixTime, lpFileTime);
	
	return true; 

}

bool FileTimeToLocalFileTime(const FILETIME *lpLocalFileTime, System::FILETIME * lpFileTime)
{
	
	time_t rawtime  = FileTime_to_POSIX((System::FILETIME *)lpLocalFileTime);
	struct tm * ptm = localtime(&rawtime);
	time_t UnixTime = mktime(ptm);
	UnixTimeToFileTime(UnixTime, lpFileTime);

	return true; 

}

void GetSystemTime(System::SYSTEMTIME * lpSystemTime)
{
	
	time_t CurrTime = time(NULL);
	tm *ttm = localtime(&CurrTime);

	lpSystemTime->wYear   = ttm->tm_year + 1900;
	lpSystemTime->wMonth  = ttm->tm_mon + 1;
	lpSystemTime->wDay    = ttm->tm_mday;
	lpSystemTime->wHour   = ttm->tm_hour;
	lpSystemTime->wMinute = ttm->tm_min;
	lpSystemTime->wSecond = ttm->tm_sec;

}

} // System
