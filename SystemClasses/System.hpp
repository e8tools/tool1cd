#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifndef DELPHI_PACKAGE
#define DELPHI_PACKAGE
#endif // DELPHI_PACKAGE

#ifndef __fastcall
#define __fastcall
#endif // __fastcall

#include <cstdint>
#include <vector>
#include <climits>

namespace System {

typedef uint32_t LongWord;
typedef uint16_t Word;
typedef uint16_t WORD;
typedef uint8_t Byte;
#ifndef __int64
typedef uint64_t __int64;
#endif // __int62
typedef char16_t WCHART;
typedef char *LPSTR;

const unsigned int MAXUINT = UINT_MAX;
const int MaxInt = INT_MAX;
const int MAX_PATH = 260;

typedef struct _FILETIME {
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

bool SystemTimeToFileTime(const SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime);
bool LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime);
bool FileTimeToLocalFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime);

void GetSystemTime(LPSYSTEMTIME lpSystemTime);


struct TGUID {
  LongWord D1;
  Word D2;
  Word D3;
  Byte D4[8];
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

