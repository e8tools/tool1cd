#ifndef SYSTEM_CLASSES_TFSTREAM
#define SYSTEM_CLASSES_TFSTREAM

#include "TStream.hpp"
#include "String.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <boost/filesystem.hpp>

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

	TFileStream(const String &FileName, const uint16_t fileMode);
	TFileStream(const boost::filesystem::path &path, const uint16_t fileMode);

	virtual ~TFileStream();

	int16_t GetHandle() const;

	virtual void Close();
	virtual void Reopen();
	virtual int64_t Read(void *Buffer, int64_t Count);
	virtual int64_t Write(const void *Buffer, int64_t Count);

protected:
	std::string filename;
	std::shared_ptr<std::fstream> _stream;
};

} // Classes
} // System

#endif
