/*
 * TableFileStream.h
 *
 *  Created on: 24 июн. 2017 г.
 *
 */

#ifndef SRC_CTOOL1CD_TABLEFILESTREAM_H_
#define SRC_CTOOL1CD_TABLEFILESTREAM_H_

#include "Common.h"
#include "Table.h"

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFileStream : public TStream
{
private:
	int64_t curoffset;
	table_file* tablefile;
	TStream** streams;
public:
	TableFileStream(table_file* tf);
	virtual ~TableFileStream();

	virtual int32_t Read(void *Buffer, int32_t Count);
	virtual int32_t Read(System::DynamicArray<System::t::Byte> Buffer, int32_t Offset, int32_t Count);
	virtual int32_t Write(const void *Buffer, int32_t Count){throw(Exception(L"Write read-only stream"));}
	virtual int32_t Write(const System::DynamicArray<System::t::Byte> Buffer, int32_t Offset, int32_t Count){throw(Exception(L"Write read-only stream"));}
	virtual int32_t Seek(int32_t Offset, System::Word Origin);
	virtual int64_t Seek(const int64_t Offset, TSeekOrigin Origin);
};


#endif /* SRC_CTOOL1CD_TABLEFILESTREAM_H_ */
