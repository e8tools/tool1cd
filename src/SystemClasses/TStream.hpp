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

	virtual int64_t Read(void *Buffer, int64_t Count) = 0;

	virtual int64_t Seek(const int64_t offset, const TSeekOrigin Origin);

	virtual int64_t Write(const void *Buffer, int64_t Count) = 0;

	virtual int64_t ReadBuffer(void *Buffer, int64_t Count);

	virtual int64_t Read(std::vector<uint8_t> &Buffer, int64_t Count);

	virtual int64_t CopyFrom(TStream *Source, const int64_t Count);

	virtual int64_t WriteBuffer(const void *Buffer, const int64_t Count);

	virtual int64_t Write(const std::vector<uint8_t> &Buffer, const int64_t Count);

	virtual void    WriteString(const std::string &string);

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

	virtual int64_t Read(void *Buffer, int64_t Count) override ;

	virtual int64_t Write(const void *Buffer, int64_t Count) override ;

	virtual ~TWrapperStream();

private:
	std::shared_ptr<std::iostream> _stream;
};

} // Classes

} // System

using namespace System::Classes;

#endif
