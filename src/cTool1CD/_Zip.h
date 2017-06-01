//---------------------------------------------------------------------------

#ifndef ZipH
#define ZipH

//#include <Classes.hpp>
//#include <vcl.h>

//---------------------------------------------------------------------------
void __fastcall InflateStream(TStream* CompressedStream, TStream* UnCompressedStream);
void __fastcall DeflateStream(TStream* UncompressedStream, TStream* CompressedStream);

#endif


