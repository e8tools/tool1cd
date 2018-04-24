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
#ifndef SYSTEM_CLASSES_TFSTREAM
#define SYSTEM_CLASSES_TFSTREAM

#include "TStream.hpp"
#include "String.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <boost/filesystem.hpp>

namespace System {

namespace Classes {

const uint16_t fmOpenRead       = 0x0000;
const uint16_t fmOpenReadWrite  = 0x0002;
const uint16_t fmShareDenyNone  = 0x0040;
const uint16_t fmShareDenyWrite = 0x0020;
const uint16_t fmShareDenyRead  = 0x0030;
const uint16_t fmCreate         = 0xFFFF;

class TFileStream : public TWrapperStream
{
public:

	TFileStream(const std::string &FileName, const uint16_t fileMode);
	TFileStream(const boost::filesystem::path &path, const uint16_t fileMode);

	virtual ~TFileStream();

	int16_t GetHandle() const;

	virtual void Close() override;
	virtual void Reopen();
	virtual int64_t Read(void *Buffer, int64_t Count) override;
	virtual int64_t Write(const void *Buffer, int64_t Count) override;

protected:
	std::string filename;
	std::shared_ptr<std::fstream> _stream;
};

} // Classes
} // System

#endif
