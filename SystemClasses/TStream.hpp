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

public:

	virtual __fastcall int64_t GetSize() const
	{
		return 0;
	}

	virtual __fastcall void SetSize(int64_t NewSize)
	{

	}

	virtual __fastcall int64_t GetPosition() const
	{
		return 0;
	}

	virtual __fastcall void SetPosition(uint64_t NewPosition)
	{
		Seek(NewPosition, soFromBeginning);
	}

public:

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count)
	{
		return 0;
	}

	virtual __fastcall int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{
		return 0;
	}

	virtual __fastcall int64_t Write(const void *Buffer, const int64_t Count)
	{
		return 0;
	}

	virtual __fastcall int64_t ReadBuffer(void *Buffer, int64_t Count)
	{
		return Read(Buffer, Count);
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
			return CopyFrom(Source, Source->GetSize());
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
			return Write(Buffer.data(), Buffer.size());
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
	{}

	TWrapperStream(std::shared_ptr<std::iostream> &&stream)
		: _stream(stream)
	{}

	void reset(std::iostream *stream)
	{
		_stream.reset(stream);
	}

public:

	virtual __fastcall int64_t GetPosition() const
	{
		return _stream->tellg();
	}

	virtual __fastcall int64_t GetSize() const
	{
		auto oldP = _stream->tellg();
		_stream->seekg(0, soFromEnd);
		auto lastP = _stream->tellg();
		_stream->seekg(oldP, soFromBeginning);
		return lastP;
	}


	virtual __fastcall int64_t Read(void *Buffer, int64_t Count)
	{
		_stream->read((char*)Buffer, Count);
		return _stream->gcount();
	}

	virtual __fastcall int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{
		std::ios_base::seekdir native = Origin;
		_stream->seekg(offset, native);
		_stream->seekp(offset, native);
		return _stream->tellg();
	}

	virtual __fastcall int64_t Write(const void *Buffer, const int64_t Count)
	{
		_stream->write((char*)Buffer, Count);
		_stream->flush();
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
