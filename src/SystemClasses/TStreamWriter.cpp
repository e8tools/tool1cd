#include "TStreamWriter.hpp"
#include "TFileStream.hpp"

namespace System {

namespace Classes {


TStreamWriter::TStreamWriter(const String &Filename, TEncoding* Encoding, int BufferSize)
	: stream(new TFileStream(Filename, fmOpenReadWrite))
{
}

TStreamWriter::TStreamWriter(TStream *stream, TEncoding* Encoding, int BufferSize)
	: stream(stream)
{
}

void TStreamWriter::Write(const String &s)
{
	// TODO:: TEncoding::Convert()
	stream->Write(s.c_str(), s.size());
}

} // Classes

} // System
