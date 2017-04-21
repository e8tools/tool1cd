#ifndef SYSTEM_CLASSES_TSTREAM_HPP
#define SYSTEM_CLASSES_TSTREAM_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include "System.hpp"
#include "DynamicArray.hpp"

namespace System {

namespace Classes {

typedef std::ios_base::seekdir TSeekOrigin;

const TSeekOrigin soBeginning = std::ios_base::beg;
const TSeekOrigin soCurrent = std::ios_base::cur;
const TSeekOrigin soEnd = std::ios_base::end;
const TSeekOrigin soFromBeginning = soBeginning;
const TSeekOrigin soFromCurrent = std::ios_base::cur;
const TSeekOrigin soFromEnd = std::ios_base::end;

class TStream
{
protected:
	int64_t m_position;
	int64_t m_size;

public:

	virtual __fastcall int64_t GetSize() const
	{
		return m_size;
	}

	virtual __fastcall void SetSize(int64_t NewSize)
	{
		// TODO: TStream::SetSize
		m_size = NewSize;
	}

	virtual __fastcall int64_t GetPosition() const
	{
		return m_position;
	}

	virtual __fastcall void SetPosition(int64_t NewPosition)
	{
		m_position = NewPosition;
	}

public:

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count)
	{
		return 0;
	}

	virtual __fastcall int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{
		if (Origin == soFromBeginning) {
			m_position = offset;
		} else if (Origin == soFromEnd) {
			m_position = GetSize() - offset;
		} else {
			m_position += offset;
		}
		return m_position;
	}

	virtual __fastcall int64_t Write(const void *Buffer, int64_t Count)
	{
		return 0;
	}

	virtual __fastcall int64_t ReadBuffer(void *Buffer, int64_t Count)
	{
		auto data_read = Read(Buffer, Count);
		return data_read;
	}

	virtual __fastcall int64_t Read(System::DynamicArray<System::Byte> &Buffer, int64_t Count)
	{
		auto old_size = Buffer.size();
		Buffer.resize(old_size + Count);
		return Read(Buffer.data() + old_size, Count);
	}

	virtual __fastcall int64_t CopyFrom(TStream *Source, const int64_t Count)
	{
		if (Count == 0) {
			auto data_size = Source->GetSize();
			if (data_size != 0) {
				return CopyFrom(Source, data_size);
			}
			return 0;
		}
		System::DynamicArray<System::Byte> _data;
		auto resultCount = Source->Read(_data, Count);
		return Write(_data, resultCount);
	}

	virtual __fastcall int64_t WriteBuffer(const void *Buffer, const int64_t Count)
	{
		auto result = Write(Buffer, Count);
		if (result != Count) {
			// Throw EWriteError
		}
		return result;
	}

	virtual __fastcall int64_t Write(const System::DynamicArray<System::Byte> &Buffer, const int64_t Count)
	{
		if (Count == 0) {
			auto data_size = Buffer.size();
			if (data_size != 0) {
				return Write(Buffer.data(), data_size);
			}
			return 0;
		}
		return Write(Buffer.data(), Count);
	}

	virtual __fastcall ~TStream() {}
};

class THandleStream : public TStream
{
	virtual __fastcall ~THandleStream() {}
};

class TWrapperStream : public TStream {
protected:

	TWrapperStream()
	{
	}

	TWrapperStream(const std::shared_ptr<std::iostream> &stream)
		: _stream(stream)
	{
		init_size();
	}

	TWrapperStream(std::shared_ptr<std::iostream> &&stream)
		: _stream(stream)
	{
		init_size();
	}

	void reset(std::iostream *stream)
	{
		_stream.reset(stream);
		init_size();
		m_position = 0;
	}

	void init_size()
	{
		_stream->seekg(0, std::ios_base::end);
		m_size = _stream->tellg();
		_stream->seekg(0, std::ios_base::beg);
	}

public:

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count)
	{
		_stream->seekg(GetPosition(), std::ios_base::beg);
		_stream->read((char*)Buffer, Count);
		auto data_read = _stream->gcount();
		m_position += data_read;
		return data_read;
	}

	virtual __fastcall int64_t Write(const void *Buffer, const int64_t Count)
	{
		_stream->seekp(GetPosition(), std::ios_base::beg);
		_stream->write((char*)Buffer, Count);
		_stream->flush();
		m_position += Count;
		if (m_position > m_size) {
			m_size = m_position;
		}
		return Count;
	}

	virtual __fastcall ~TWrapperStream() {}

private:
	std::shared_ptr<std::iostream> _stream;
};

} // Classes

} // System

using namespace System::Classes;

#endif
