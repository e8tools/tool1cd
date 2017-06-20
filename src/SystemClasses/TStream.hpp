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

const TSeekOrigin soBeginning     = std::ios_base::beg;
const TSeekOrigin soCurrent       = std::ios_base::cur;
const TSeekOrigin soEnd           = std::ios_base::end;
const TSeekOrigin soFromBeginning = soBeginning;
const TSeekOrigin soFromCurrent   = std::ios_base::cur;
const TSeekOrigin soFromEnd       = std::ios_base::end;

class TStream
{
protected:
	size_t m_position;
	size_t m_size;

public:

	TStream();

	//virtual int64_t GetSize() const;
	virtual size_t GetSize() const;

	//virtual void SetSize(int64_t NewSize);
	virtual void SetSize(size_t NewSize);

	virtual size_t GetPosition() const;

	virtual void SetPosition(size_t NewPosition);

public:

	virtual size_t Read(void *Buffer, size_t Count);

	virtual size_t Seek(const size_t offset, const TSeekOrigin Origin);

	virtual size_t Write(const void *Buffer, size_t Count);

	virtual size_t ReadBuffer(void *Buffer, size_t Count);

	virtual size_t Read(System::DynamicArray<System::t::Byte> &Buffer, size_t Count);

	virtual size_t CopyFrom(TStream *Source, const size_t Count);

	virtual size_t WriteBuffer(const void *Buffer, const size_t Count);

	virtual size_t Write(const System::DynamicArray<System::t::Byte> &Buffer, const size_t Count);

	virtual void   Close();

	virtual ~TStream();
};

class THandleStream : public TStream
{
	virtual ~THandleStream();
};

class TWrapperStream : public TStream
{
protected:

	TWrapperStream();

	TWrapperStream(const std::shared_ptr<std::iostream> &stream);

	TWrapperStream(std::shared_ptr<std::iostream> &&stream);

	void reset(std::iostream *stream);
	void reopen(const std::shared_ptr<std::iostream> &stream);

	void init_size();

public:

	virtual size_t Read(void *Buffer, size_t Count);

	virtual size_t Write(const void *Buffer, size_t Count);

	virtual ~TWrapperStream();

private:
	std::shared_ptr<std::iostream> _stream;
};

} // Classes

} // System

using namespace System::Classes;

#endif
