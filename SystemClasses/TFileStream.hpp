#ifndef SYSTEM_CLASSES_TFSTREAM
#define SYSTEM_CLASSES_TFSTREAM

#include "TStream.hpp"
#include "String.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace System {

namespace Classes {

const uint16_t fmOpenRead       = 0x0000;
const uint16_t fmOpenReadWrite  = 0x0002;
const uint16_t fmShareDenyNone  = 0x0040;
const uint16_t fmShareDenyWrite = 0x0020;
const uint16_t fmShareDenyRead  = 0x0030;
const uint16_t fmCreate         = 0xFFFF;

typedef uint16_t HANDLE;

class TFileStream : public TWrapperStream
{
public:
	TFileStream(const String &FileName, const uint16_t fileMode)
		: TWrapperStream()
	{

		std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
		if (fileMode == fmCreate) {
			mode |= std::ios::trunc;
		}

		reset(new boost::filesystem::fstream(boost::filesystem::path(FileName.c_str()), mode));

	}

	virtual __fastcall ~TFileStream()
	{
	}

	int16_t GetHandle() const
	{
		return 0;
	}

};

} // Classes
} // System

#endif
