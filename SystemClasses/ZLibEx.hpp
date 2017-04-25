#ifndef ZlibexHPP
#define ZlibexHPP

#include "System.hpp"
#include "SysInit.hpp"
#include "System.SysUtils.hpp"
#include "System.Classes.hpp"

namespace Zlibex
{

void ZDecompressStream(System::Classes::TStream* inStream, System::Classes::TStream* outStream);

}

using namespace Zlibex;

#endif	// ZlibexHPP
