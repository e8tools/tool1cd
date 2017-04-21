#ifndef SYSTEM_CLASSES_TMEMORYSTREAM
#define SYSTEM_CLASSES_TMEMORYSTREAM

#include "System.hpp"
#include "DynamicArray.hpp"
#include "TStream.hpp"
#include <cstring>

namespace System {

namespace Classes {

class TMemoryStream : public TStream
{
public:

	TMemoryStream();
	__fastcall ~TMemoryStream();

	virtual __fastcall int64_t GetSize() const;

	virtual __fastcall void    SetSize(int64_t NewSize);

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count);

	virtual __fastcall int64_t Write(const void *Buffer, const int64_t Count);

	const DynamicArray<Byte>  &GetBytes() const;

	void                      *GetMemory();

private:

	void EnsureSize(size_t size);

	DynamicArray<Byte> _data;
};

class TBytesStream : public TMemoryStream
{
public:
	TBytesStream(const DynamicArray<Byte> &initial);

	virtual __fastcall ~TBytesStream();
};

} // Classes

} // System

#endif
