#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define DELPHI_PACKAGE

#define __fastcall

#include <cstdint>
#include <vector>

namespace System {

typedef uint32_t LongWord;
typedef uint16_t Word;
typedef uint8_t Byte;


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

}

#endif

