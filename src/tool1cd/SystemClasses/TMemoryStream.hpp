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
#ifndef SYSTEM_CLASSES_TMEMORYSTREAM
#define SYSTEM_CLASSES_TMEMORYSTREAM

#include <cstring>
#include <vector>

#include "System.hpp"
#include "TStream.hpp"

namespace System {

namespace Classes {

class TMemoryStream : public TStream
{
public:

	TMemoryStream();
	virtual ~TMemoryStream();

	virtual int64_t GetSize() const override;

	virtual void    SetSize(int64_t NewSize) override;

	virtual int64_t Read(void *Buffer, int64_t Count) override;

	virtual int64_t Write(const void *Buffer, int64_t Count) override;

	const std::vector<uint8_t> &GetBytes() const;

	void *GetMemory();

private:

	void EnsureSize(size_t size);

	std::vector<uint8_t> _data;
};

class TBytesStream : public TMemoryStream
{
public:
	TBytesStream(const std::vector<uint8_t> &initial);

	virtual ~TBytesStream();
};

} // Classes

} // System

#endif
