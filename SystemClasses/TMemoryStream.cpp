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
	m_size = NewSize;
	if (m_position > m_size) {
		m_position = m_size;
	}
}

int64_t TMemoryStream::Read(void *Buffer, int64_t Count)
{
	int64_t toRead = Count;
	if (_data.size() <= m_position + toRead) {
		toRead = _data.size() - m_position;
	}
	if (toRead > 0) {
		memcpy(Buffer, _data.data() + m_position, toRead);
		m_position += toRead;
		return toRead;
	}
	return 0;
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
