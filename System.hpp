#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define DELPHI_PACKAGE

#define __fastcall

#include <cstdint>
#include <vector>
#include <climits>

namespace System {

typedef uint32_t LongWord;
typedef uint16_t Word;
typedef uint8_t Byte;
typedef uint64_t __int64;
typedef wchar_t *LPWSTR;
typedef char *LPSTR;

const unsigned int MAXUINT = UINT_MAX;

struct FILETIME {};

struct TGUID {
  LongWord D1;
  Word D2;
  Word D3;
  Byte D4[8];
};

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

