#include "TStreamReader.hpp"

namespace System {

namespace Classes {


TStreamReader::TStreamReader(TStream *stream, bool DetectBOM)
	: stream(stream)
{
}

int TStreamReader::Read()
{
	uint8_t data;
	auto data_read = stream->Read(&data, 1);
	if (data_read != 1) {
		return -1;
	}
	return data;
}


} // Classes

} // System
