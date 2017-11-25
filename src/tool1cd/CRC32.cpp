#include "CRC32.h"
#include <boost/crc.hpp>

uint32_t _crc32(TStream* str)
{
	str->Seek(0, soFromBeginning);
	std::streamsize const  buffer_size = 1024;
	boost::crc_32_type result;
	while(true) {
		char  buffer[ buffer_size ];
		auto readBytes = str->Read( &buffer, buffer_size );
		if(!readBytes) {
			break;
		}
		result.process_bytes( buffer, readBytes );
	}
	return result.checksum();
}
