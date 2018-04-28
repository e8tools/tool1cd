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
#include "TFileStream.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace System {

namespace Classes {

TFileStream::TFileStream(const std::string &FileName, const uint16_t fileMode)
	: TWrapperStream(), filename(FileName)
{
	std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
	if (fileMode == fmCreate) {
		mode |= std::ios::trunc;
	}
	_stream = std::make_shared<boost::filesystem::fstream>(boost::filesystem::path(filename), mode);
	reopen(_stream);
	m_position = 0;
}

TFileStream::TFileStream(const boost::filesystem::path &path, const uint16_t fileMode)
	: TWrapperStream(), filename(path.string())
{
	std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
	if (fileMode == fmCreate) {
		mode |= std::ios::trunc;
	}
	_stream = std::make_shared<boost::filesystem::fstream>(path, mode);
	reopen(_stream);
	m_position = 0;
}


TFileStream::~TFileStream()
{
	Close();
}

int16_t TFileStream::GetHandle() const
{
	return 0;
}

void TFileStream::Reopen()
{
	if (_stream) {
		if (_stream->is_open()) {
			return;
		}
	}
	std::ios_base::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
	_stream = std::make_shared<boost::filesystem::fstream>(boost::filesystem::path(filename), mode);
	reopen(_stream);
}


void TFileStream::Close()
{
	_stream->close();
	_stream.reset();
}

int64_t TFileStream::Read(void *Buffer, int64_t Count)
{
	Reopen();
	return TWrapperStream::Read(Buffer, Count);
}

int64_t TFileStream::Write(const void *Buffer, int64_t Count)
{
	Reopen();
	return TWrapperStream::Write(Buffer, Count);
}


} // Classes

} // System
