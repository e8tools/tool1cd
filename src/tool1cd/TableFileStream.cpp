/*
 * TableFileStream.cpp
 *
 *  Created on: 24 июн. 2017 г.
 *
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
int32_t TableFileStream::Read(std::vector<System::t::Byte> Buffer, int32_t Offset, int32_t Count)
{
	// TODO: реализовать TableFileStream::Read(System::DynamicArray<System::t::Byte> Buffer, int32_t Offset, int32_t Count)
	return 0;
}

//---------------------------------------------------------------------------
int32_t TableFileStream::Seek(int32_t Offset, System::Word Origin)
{
	// TODO: реализовать TableFileStream::Seek(int32_t Offset, System::Word Origin)
	return 0;
}

//---------------------------------------------------------------------------
int64_t TableFileStream::Seek(const int64_t Offset, TSeekOrigin Origin)
{
	// TODO: реализовать TableFileStream::Seek(const int64_t Offset, TSeekOrigin Origin)
	return 0;
}



