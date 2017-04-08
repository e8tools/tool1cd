#ifndef SYSTEM_HPP
#define SYSTEM_HPP


#define DELPHI_PACKAGE

#define _DELPHI_STRING_UNICODE

#define __fastcall

#include <cstdint>
#include <vector>

namespace System {

typedef uint16_t Word;
typedef uint8_t Byte;

template <typename T>
class DynamicArray : std::vector<T> {

};

}

#endif

