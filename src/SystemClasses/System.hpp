#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifndef DELPHI_PACKAGE
#define DELPHI_PACKAGE
#endif // DELPHI_PACKAGE

#include <cstdint>
#include <vector>
#include <climits>
//#include <boost/date_time/filetime_functions.hpp>
#include <boost/date_time/c_time.hpp>

//#include <Winnt.h>
//#include <Winbase.h>


#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

#define Int32x32To64(a, b)  ((__int64)(((__int64)((long)(a))) * ((long)(b))))
#define UInt32x32To64(a, b) ((unsigned __int64)(((unsigned __int64)((unsigned int)(a))) * ((unsigned int)(b))))


namespace System {

typedef uint32_t         LongWord;
typedef uint16_t         Word;
typedef uint16_t         WORD;
typedef unsigned long    DWORD;
typedef char16_t         WCHART;
typedef char             *LPSTR;
typedef __int64          LONGLONG;
typedef unsigned __int64 ULONGLONG;
typedef long             LONG;



namespace t { // в msvc конфликтует с zlib
typedef uint8_t Byte;
}

const int MAX_PATH = 260;

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



bool SystemTimeToFileTime(const SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime);
bool LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime);
bool FileTimeToLocalFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime);

void GetSystemTime(LPSYSTEMTIME lpSystemTime);


struct TGUID {
  LongWord D1;
  Word     D2;
  Word     D3;
  t::Byte  D4[8];
};

bool operator == (const TGUID &a, const TGUID &b);

typedef TGUID GUID;

enum TReplaceFlags {
	rfReplaceAll = 1,
	rfIgnoreCase = 2
};

template<typename T>
void LocalFree(T *anyvalue)
{
	// Do nothing
}

} // System

#endif

