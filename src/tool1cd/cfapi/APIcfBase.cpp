#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <array>
#include <stdexcept>

#include "APIcfBase.h"

using namespace System;

//---------------------------------------------------------------------------
// возвращает секунды от эпохи UNIX
// https://stackoverflow.com/questions/6161776/convert-windows-filetime-to-second-in-unix-linux
//
unsigned WindowsTickToUnixSeconds(long long windowsTicks)
{
	return (unsigned)(windowsTicks / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
}

//---------------------------------------------------------------------------
//преобразование времени
void V8timeToFileTime(const int64_t* v8t, FILETIME* ft) {

	FILETIME lft;

	int64_t t = *v8t;
	t -= EPOCH_START_WIN; //504911232000000 = ((365 * 4 + 1) * 100 - 3) * 4 * 24 * 60 * 60 * 10000
	t *= 1000;
	*(int64_t*)&lft = t;
	LocalFileTimeToFileTime(&lft, ft);

}

//---------------------------------------------------------------------------
// обратное преобразование времени
void FileTimeToV8time(const FILETIME* ft, int64_t* v8t) {

	FILETIME lft;

	FileTimeToLocalFileTime(ft, &lft);
	int64_t t = *(int64_t*)&lft;
	t /= 1000;
	t += EPOCH_START_WIN; //504911232000000 = ((365 * 4 + 1) * 100 - 3) * 4 * 24 * 60 * 60 * 10000
	*v8t = t;
}

//---------------------------------------------------------------------------
// установка текущего времени
void setCurrentTime(int64_t* v8t)
{
	SYSTEMTIME st;
	FILETIME ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	FileTimeToV8time(&ft, v8t);

}
