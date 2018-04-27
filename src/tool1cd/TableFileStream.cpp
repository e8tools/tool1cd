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

#include "TableFileStream.h"

//---------------------------------------------------------------------------
TableFileStream::TableFileStream(TableFile* tf)
{
	curoffset = 0;
	tablefile = tf;
	streams = new TStream*[tablefile->maxpartno + 1];
	for(uint32_t i = 0; i <= tablefile->maxpartno; ++i) streams[i] = nullptr;
}

//---------------------------------------------------------------------------
TableFileStream::~TableFileStream()
{
	for(uint32_t i = 0; i <= tablefile->maxpartno; ++i) delete streams[i];
	delete[] streams;
}

//---------------------------------------------------------------------------
int64_t TableFileStream::Read(void *Buffer, int64_t Count)
{
	uint32_t nbf; // индекс текущего table_blob_file в tablefile->addr
	uint32_t ibf; // индекс внутри table_blob_file
	uint32_t curoff; // curoffset
	table_blob_file* addr;
	TStream* str;
	int64_t curlen;
	char* buf;

	buf = (char*)Buffer;
	addr = tablefile->addr;
	for(nbf = 0, curoff = 0; nbf <= tablefile->maxpartno; ++nbf)
	{
		if(curoff + addr[nbf].blob_length > curoffset)
		{
			ibf = curoffset - curoff;
			break;
		}
		curoff += addr[nbf].blob_length;
	}
	if(nbf > tablefile->maxpartno) return 0;

	while(Count)
	{
		curlen = std::min(Count, (int64_t)(addr[nbf].blob_length - ibf));
		str = streams[nbf];
		if(!str)
		{
			if(ibf == 0 && addr[nbf].blob_length == curlen)
			{

			}
		}

		if(str)
		{
			str->Seek(ibf, soBeginning);
			str->Read(buf, curlen);
		}
		else
		{

		}
		buf += curlen;
		Count -= curlen;
	}

	// TODO: Возвращаемое значение TableFileStream::Read(void *Buffer, int32_t Count)
	return 0;
}

//---------------------------------------------------------------------------
int32_t TableFileStream::Read(std::vector<uint8_t> Buffer, int32_t Offset, int32_t Count)
{
	// TODO: реализовать TableFileStream::Read(System::DynamicArray<System::t::Byte> Buffer, int32_t Offset, int32_t Count)
	return 0;
}

//---------------------------------------------------------------------------
int64_t TableFileStream::Seek(const int64_t Offset, const TSeekOrigin Origin)
{
	// TODO: реализовать TableFileStream::Seek(const int64_t Offset, TSeekOrigin Origin)
	return 0;
}



