#ifndef SYSTEM_CLASSES_TSTREAM_HPP
#define SYSTEM_CLASSES_TSTREAM_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#include "System.hpp"

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
	int64_t m_position;
	int64_t m_size;

public:

	TStream();

	virtual int64_t GetSize() const;

	virtual void SetSize(int64_t NewSize);

	virtual int64_t GetPosition() const;

	virtual void SetPosition(int64_t NewPosition);

public:

	virtual int64_t Read(void *Buffer, int64_t Count);

	virtual int64_t Seek(const int64_t offset, const TSeekOrigin Origin);

	virtual int64_t Write(const void *Buffer, int64_t Count);

	virtual int64_t ReadBuffer(void *Buffer, int64_t Count);

	virtual int64_t Read(std::vector<System::t::Byte> &Buffer, int64_t Count);

	virtual int64_t CopyFrom(TStream *Source, const int64_t Count);

	virtual int64_t WriteBuffer(const void *Buffer, const int64_t Count);

	virtual int64_t Write(const std::vector<System::t::Byte> &Buffer, const int64_t Count);

	virtual void    Close();

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

	virtual int64_t Read(void *Buffer, int64_t Count);

	virtual int64_t Write(const void *Buffer, int64_t Count);

	virtual ~TWrapperStream();

private:
	std::shared_ptr<std::iostream> _stream;
};

} // Classes

} // System

using namespace System::Classes;

#endif
