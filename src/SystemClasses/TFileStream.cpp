#include "TFileStream.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace System {

namespace Classes {

TFileStream::TFileStream(const std::string &FileName, const uint16_t fileMode)
	: TWrapperStream(), filename(FileName)
{
	std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
	if (fileMode == fmCreate) {
		mode |= std::ios::trunc;
	}
	_stream = std::make_shared<boost::filesystem::fstream>(boost::filesystem::path(filename), mode);
	reopen(_stream);
	m_position = 0;
}

TFileStream::TFileStream(const boost::filesystem::path &path, const uint16_t fileMode)
	: TWrapperStream(), filename(path.string())
{
	std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
	if (fileMode == fmCreate) {
		mode |= std::ios::trunc;
	}
	_stream = std::make_shared<boost::filesystem::fstream>(path, mode);
	reopen(_stream);
	m_position = 0;
}


TFileStream::~TFileStream()
{
	Close();
}

int16_t TFileStream::GetHandle() const
{
	return 0;
}

void TFileStream::Reopen()
{
	if (_stream) {
		if (_stream->is_open()) {
			return;
		}
	}
	std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
	_stream = std::make_shared<boost::filesystem::fstream>(boost::filesystem::path(filename), mode);
	reopen(_stream);
}


void TFileStream::Close()
{
	_stream->close();
	_stream.reset();
}

int64_t TFileStream::Read(void *Buffer, int64_t Count)
{
	Reopen();
	return TWrapperStream::Read(Buffer, Count);
}

int64_t TFileStream::Write(const void *Buffer, int64_t Count)
{
	Reopen();
	return TWrapperStream::Write(Buffer, Count);
}


} // Classes

} // System
