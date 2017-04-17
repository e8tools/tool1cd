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
	~TMemoryStream() {}

	virtual int64_t GetSize() const
	{
		return _data.size();
	}

	virtual void SetSize(int64_t NewSize)
	{
		_data.resize(NewSize);
	}

	virtual int64_t Read(void *Buffer, int64_t Count)
	{
		memcpy(Buffer, _data.data() + _pos, Count);
		_pos += Count;
		return Count;
	}

	virtual int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
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

	virtual int64_t Write(const void *Buffer, const int64_t Count)
	{
		auto buf = reinterpret_cast<const Byte*>(Buffer);
		EnsureSize(_pos + Count);
		for (auto i = Count; i; i--) {
			_data[_pos++] = *buf++;
		}
		return Count;
	}

private:

	void EnsureSize(int size)
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

};

} // Classes

} // System

#endif
