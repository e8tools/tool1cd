#ifndef SYSTEM_TSTREAMWRITER_HPP
#define SYSTEM_TSTREAMWRITER_HPP

#include "TStream.hpp"
#include "System.SysUtils.hpp"
#include "String.hpp"

namespace System {

namespace Classes {

class TStreamWriter
{
public:

	TStreamWriter(const String  &Filename, TEncoding* Encoding, int BufferSize);
	TStreamWriter(      TStream *stream,   TEncoding* Encoding, int BufferSize);

	void Write   (const String   &s);

private:
	TStream *stream;
};

} // Classes

} // System

#endif
