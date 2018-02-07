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
