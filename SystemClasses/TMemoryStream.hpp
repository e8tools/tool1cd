#ifndef SYSTEM_CLASSES_TMEMORYSTREAM
#define SYSTEM_CLASSES_TMEMORYSTREAM

#include "System.hpp"
#include "DynamicArray.hpp"
#include "TStream.hpp"
#include <cstring>

namespace System {

namespace Classes {

class TMemoryStream : public TStream {
public:
	TMemoryStream() {}
	__fastcall ~TMemoryStream() {}

	virtual __fastcall int64_t GetSize() const
	{
		return _data.size();
	}

	virtual __fastcall void SetSize(int64_t NewSize)
	{
		_data.resize(NewSize);
	}

	virtual __fastcall int64_t Read(void *Buffer, int64_t Count)
	{
		memcpy(Buffer, _data.data() + _pos, Count);
		_pos += Count;
		return Count;
	}

	virtual __fastcall int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{
		if (Origin == soBeginning) {
			_pos = offset;
		} else if (Origin == soCurrent) {
			_pos += offset;
		} else if (Origin == soEnd) {
			_pos = _data.size() - offset;
		}
		return _pos;
	}

	virtual __fastcall int64_t Write(const void *Buffer, const int64_t Count)
	{
		auto buf = reinterpret_cast<const Byte*>(Buffer);
		EnsureSize(_pos + Count);
		for (auto i = Count; i; i--) {
			_data[_pos++] = *buf++;
		}
		return Count;
	}

	const DynamicArray<Byte> &GetBytes() const
	{
		return _data;
	}

	void *GetMemory()
	{
		return _data.data();
	}

private:

	void EnsureSize(size_t size)
	{
		if (_data.size() < size) {
			_data.resize(size);
		}
	}

	DynamicArray<Byte> _data;
	int _pos;
};

class TBytesStream : public TMemoryStream
{
public:
	TBytesStream(const DynamicArray<Byte> &initial) {}

	virtual __fastcall ~TBytesStream() {}
};

} // Classes

} // System

#endif
