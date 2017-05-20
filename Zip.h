//---------------------------------------------------------------------------

#ifndef ZipH
#define ZipH

#include <System.Classes.hpp>

//---------------------------------------------------------------------------
//void InflateStream(TStream* CompressedStream, TStream* UnCompressedStream);
//void DeflateStream(TStream* UncompressedStream, TStream* CompressedStream);

namespace Abunzprc
{
void InflateStream(Classes::TStream* CompressedStream, Classes::TStream* UnCompressedStream);
}
using namespace Abunzprc;

namespace Abzipprc
{
void DeflateStream(Classes::TStream* UncompressedStream, Classes::TStream* CompressedStream);
}
using namespace Abzipprc;

#endif
