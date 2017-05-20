#include "TStreamReader.hpp"

namespace System {

namespace Classes {


TStreamReader::TStreamReader(TStream *stream, bool DetectBOM)
	: stream(stream)
{
}

int TStreamReader::Read()
{
	t::Byte data;
	auto data_read = stream->Read(&data, 1);
	if (data_read != 1) {
		return -1;
	}
	return data;
}

String TStreamReader::ReadLine()
{
	// TODO: TStreamReader::ReadLine()
	return String();
}


} // Classes

} // System
