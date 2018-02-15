#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifndef DELPHI_PACKAGE
#define DELPHI_PACKAGE
#endif // DELPHI_PACKAGE

#include <cstdint>
#include <vector>
#include <climits>
#include <ctime>

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

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

} FILETIME, *LPFILETIME;

typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;


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
} LARGE_INTEGER, *PLARGE_INTEGER;


int64_t Int32x32To64(int64_t a, int64_t b);
uint64_t UInt32x32To64(int64_t a, int64_t b);

uint64_t FileTime_to_POSIX(LPFILETIME pft);
void UnixTimeToFileTime(time_t t, LPFILETIME pft);

bool SystemTimeToFileTime(const SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime);
bool LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime);
bool FileTimeToLocalFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime);

void GetSystemTime(LPSYSTEMTIME lpSystemTime);

enum TReplaceFlags {
	rfReplaceAll = 1,
	rfIgnoreCase = 2
};

} // System

using System::WCHART;

#include "String.hpp"

#endif

