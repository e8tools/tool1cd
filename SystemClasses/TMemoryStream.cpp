#include "TMemoryStream.hpp"

#include "System.hpp"
#include "DynamicArray.hpp"
#include "TStream.hpp"
#include <cstring>

namespace System {

namespace Classes {

TMemoryStream::TMemoryStream() {}
TMemoryStream::~TMemoryStream() {}

int64_t TMemoryStream::GetSize() const
{
	return _data.size();
}

void TMemoryStream::SetSize(int64_t NewSize)
{
	_data.resize(NewSize);
}

int64_t TMemoryStream::Read(void *Buffer, int64_t Count)
{
	memcpy(Buffer, _data.data() + m_position, Count);
	m_position += Count;
	return Count;
}

int64_t TMemoryStream::Write(const void *Buffer, const int64_t Count)
{
	auto buf = reinterpret_cast<const Byte*>(Buffer);
	EnsureSize(m_position + Count);
	for (auto i = Count; i; i--) {
		_data[m_position++] = *buf++;
	}
	return Count;
}

const DynamicArray<Byte> &TMemoryStream::GetBytes() const
{
	return _data;
}

void *TMemoryStream::GetMemory()
{
	return _data.data();
}


void TMemoryStream::EnsureSize(size_t size)
{
	if (_data.size() < size) {
		_data.resize(size);
		m_size = _data.size();
	}
}

TBytesStream::TBytesStream(const DynamicArray<Byte> &initial)
{
	Write(initial.data(), initial.size());
	m_position = 0;
}

TBytesStream::~TBytesStream() {}

} // Classes

} // System
