#include "TV8FileStream.h"

using namespace System;

TV8FileStream::TV8FileStream(v8file* f, bool ownfile) : TStream(), file(f), own(ownfile)
{
	pos = 0l;
	file->streams.insert(this);
}

//---------------------------------------------------------------------------
// декструктор
TV8FileStream::~TV8FileStream()
{
	if(own) delete file;
	else file->streams.erase(this);
}

//---------------------------------------------------------------------------
// чтение буфера
int64_t TV8FileStream::Read(void *Buffer, int64_t Count)
{
	int r = file->Read(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// чтение буфера
int TV8FileStream::Read(std::vector<t::Byte> Buffer, int Offset, int Count)
{
	int r = file->Read(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// запись буфера
int64_t TV8FileStream::Write(const void *Buffer, int64_t Count)
{
	int r = file->Write(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// запись буфера
int TV8FileStream::Write(const std::vector<t::Byte> Buffer, int Offset, int Count)
{
	int r = file->Write(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// позиционирование
int64_t TV8FileStream::Seek(const int64_t Offset, TSeekOrigin Origin)
{
	int64_t len = file->GetFileLength();
	switch(Origin)
	{
		case soFromBeginning:
			if(Offset >= 0)	{
				if(Offset <= len) {
					pos = Offset;
				}
				else {
					pos = len;
				}
			}
			break;
		case soFromCurrent:
			if(pos + Offset < len) {
				pos += Offset;
			}
			else {
				pos = len;
			}
			break;
		case soFromEnd:
			if(Offset <= 0) {
				if(Offset <= len) {
					pos = len - Offset;
				}
				else {
					pos = 0;
				}
			}
			break;
	}
	return pos;
}
