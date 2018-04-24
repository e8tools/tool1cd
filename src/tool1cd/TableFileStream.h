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

#ifndef SRC_CTOOL1CD_TABLEFILESTREAM_H_
#define SRC_CTOOL1CD_TABLEFILESTREAM_H_

#include <vector>

#include "Common.h"
#include "Table.h"

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFileStream : public TStream
{
private:
	int64_t curoffset;
	TableFile* tablefile;
	TStream** streams;
public:
	TableFileStream(TableFile *tf);
	virtual ~TableFileStream();

	virtual int64_t Read(void *Buffer, int64_t Count) override;
	virtual int32_t Read(std::vector<uint8_t> Buffer, int32_t Offset, int32_t Count);
	virtual int64_t Write(const void *Buffer, int64_t Count) override { throw(System::Exception(L"Write read-only stream")); }
	virtual int32_t Write(const std::vector<uint8_t> Buffer, int32_t Offset, int32_t Count) { throw(System::Exception(L"Write read-only stream")); }
	virtual int32_t Seek(int32_t Offset, System::Word Origin);
	virtual int64_t Seek(const int64_t Offset, const TSeekOrigin Origin) override;
};


#endif /* SRC_CTOOL1CD_TABLEFILESTREAM_H_ */
