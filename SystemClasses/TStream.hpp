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

	virtual int64_t GetSize() const
	{
		return 0;
	}

	virtual void SetSize(int64_t NewSize)
	{

	}

	virtual int64_t GetPosition() const
	{
		return 0;
	}

	virtual void SetPosition(uint64_t NewPosition)
	{
		
	}

public:

	virtual int64_t Read(void *Buffer, int64_t Count)
	{

	}

	virtual int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{

	}

	virtual int64_t Write(const void *Buffer, const int64_t Count)
	{
		return 0;
	}

	virtual int64_t ReadBuffer(void *Buffer, int64_t Count)
	{
		return Read(Buffer, Count);
	}

	virtual int64_t Read(System::DynamicArray<System::Byte> &Buffer, int64_t Count)
	{
		auto old_size = Buffer.size();
		Buffer.resize(old_size + Count);
		return Read(Buffer.data() + old_size, Count);
	}

	virtual int64_t CopyFrom(TStream *Source, const int64_t Count)
	{
		if (Count == 0) {
			return CopyFrom(Source, Source->GetSize());
		}
		System::DynamicArray<System::Byte> _data;
		auto resultCount = Source->Read(_data, Count);
		return Write(_data, resultCount);
	}

	virtual int64_t WriteBuffer(const void *Buffer, const int64_t Count)
	{
		auto result = Write(Buffer, Count);
		if (result != Count) {
			// Throw EWriteError
		}
		return result;
	}

	virtual int64_t Write(const System::DynamicArray<System::Byte> &Buffer, const int64_t Count)
	{
		if (Count == 0) {
			return Write(Buffer.data(), Buffer.size());
		}
		return Write(Buffer.data(), Count);
	}

	virtual ~TStream() {}
};

class THandleStream : public TStream
{

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

	virtual int64_t GetSize() const
	{
		auto oldg = _stream->tellg();
		_stream->seekg(0, std::ios_base::end);
		auto pos = _stream->tellg();
		_stream->seekg(oldg, std::ios_base::beg);
		return pos;
	}


	virtual int64_t Read(void *Buffer, int64_t Count)
	{
		_stream->read((char*)Buffer, Count);
		return _stream->gcount();
	}

	virtual int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{
		std::ios_base::seekdir native = Origin;
		_stream->seekg(offset, native);
		_stream->seekp(offset, native);
	}

	virtual int64_t Write(const void *Buffer, const int64_t Count)
	{
		_stream->write((char*)Buffer, Count);
		_stream->flush();
		return Count;
	}

private:
	std::shared_ptr<std::iostream> _stream;
};

} // Classes

} // System

using namespace System::Classes;

#endif
