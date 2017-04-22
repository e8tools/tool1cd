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

	TStream();

	virtual __fastcall int64_t GetSize() const;

	virtual __fastcall void SetSize(int64_t NewSize);

	virtual __fastcall int64_t GetPosition() const;

	virtual __fastcall void SetPosition(int64_t NewPosition);

public:

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count);

	virtual __fastcall int64_t Seek(const int64_t offset, const TSeekOrigin Origin);

	virtual __fastcall int64_t Write(const void *Buffer, int64_t Count);

	virtual __fastcall int64_t ReadBuffer(void *Buffer, int64_t Count);

	virtual __fastcall int64_t Read(System::DynamicArray<System::Byte> &Buffer, int64_t Count);

	virtual __fastcall int64_t CopyFrom(TStream *Source, const int64_t Count);

	virtual __fastcall int64_t WriteBuffer(const void *Buffer, const int64_t Count);

	virtual __fastcall int64_t Write(const System::DynamicArray<System::Byte> &Buffer, const int64_t Count);

	virtual __fastcall ~TStream();
};

class THandleStream : public TStream
{
	virtual __fastcall ~THandleStream();
};

class TWrapperStream : public TStream
{
protected:

	TWrapperStream();

	TWrapperStream(const std::shared_ptr<std::iostream> &stream);

	TWrapperStream(std::shared_ptr<std::iostream> &&stream);

	void reset(std::iostream *stream);

	void init_size();

public:

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count);

	virtual __fastcall int64_t Write(const void *Buffer, const int64_t Count);

	virtual __fastcall ~TWrapperStream();

private:
	std::shared_ptr<std::iostream> _stream;
};

} // Classes

} // System

using namespace System::Classes;

#endif
