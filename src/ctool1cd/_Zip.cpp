//---------------------------------------------------------------------------

#pragma hdrstop

#include <IdBaseComponent.hpp>
#include <IdCompressorZLib.hpp>
#include <IdZLibCompressorBase.hpp>
#include "Zip.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

class ZLib
{
private:
	//TIdCompressorZLib* zipper;
public:
	__fastcall ZLib();
	__fastcall ~ZLib();
	void __fastcall InflateStream(TStream* CompressedStream, TStream* UnCompressedStream);
	void __fastcall DeflateStream(TStream* UncompressedStream, TStream* CompressedStream);
};

ZLib zip;

__fastcall ZLib::ZLib()
{
	//zipper = new TIdCompressorZLib;
}

__fastcall ZLib::~ZLib()
{
	//delete zipper;
	//zipper = NULL;
}

void __fastcall ZLib::InflateStream(TStream* CompressedStream, TStream* UnCompressedStream)
{
	//if(!zipper) zipper = new TIdCompressorZLib;
	TIdCompressorZLib* zipper = new TIdCompressorZLib;
	zipper->InflateStream(CompressedStream, UnCompressedStream);
	delete zipper;
}

void __fastcall ZLib::DeflateStream(TStream* UncompressedStream, TStream* CompressedStream)
{
	//if(!zipper) zipper = new TIdCompressorZLib;
	TIdCompressorZLib* zipper = new TIdCompressorZLib;

	//zipper->DeflateStream(UncompressedStream, CompressedStream, 3);
	zipper->CompressStream(UncompressedStream, CompressedStream, 8, -15, 8, 0);

	delete zipper;
}

void __fastcall InflateStream(TStream* CompressedStream, TStream* UnCompressedStream)
{
	zip.InflateStream(CompressedStream, UnCompressedStream);
}

void __fastcall DeflateStream(TStream* UncompressedStream, TStream* CompressedStream)
{
	zip.DeflateStream(UncompressedStream, CompressedStream);
}


