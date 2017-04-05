//---------------------------------------------------------------------------

#ifndef ZipH
#define ZipH

#include <System.Classes.hpp>

//---------------------------------------------------------------------------
//void __fastcall InflateStream(TStream* CompressedStream, TStream* UnCompressedStream);
//void __fastcall DeflateStream(TStream* UncompressedStream, TStream* CompressedStream);

namespace Abunzprc
{
void __fastcall InflateStream(Classes::TStream* CompressedStream, Classes::TStream* UnCompressedStream);
}
using namespace Abunzprc;

namespace Abzipprc
{
void __fastcall DeflateStream(Classes::TStream* UncompressedStream, Classes::TStream* CompressedStream);
}
using namespace Abzipprc;

#endif
