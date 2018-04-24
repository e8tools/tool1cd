/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cstring>

#include "TMemoryStream.hpp"
#include "System.hpp"
#include "TStream.hpp"

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

int64_t TMemoryStream::Write(const void *Buffer, int64_t Count)
{
	auto buf = reinterpret_cast<const uint8_t*>(Buffer);
	EnsureSize(m_position + Count);
	for (auto i = Count; i; i--) {
		_data[m_position++] = *buf++;
	}
	return Count;
}

const std::vector<uint8_t> &TMemoryStream::GetBytes() const
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

TBytesStream::TBytesStream(const std::vector<uint8_t> &initial)
{
	Write(initial.data(), initial.size());
	m_position = 0;
}

TBytesStream::~TBytesStream() {}

} // Classes

} // System
