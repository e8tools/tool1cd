/*
 * TableFileStream.h
 *
 *  Created on: 24 июн. 2017 г.
 *
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
	virtual int32_t Read(std::vector<System::t::Byte> Buffer, int32_t Offset, int32_t Count);
	virtual int64_t Write(const void *Buffer, int64_t Count) override { throw(System::Exception(L"Write read-only stream")); }
	virtual int32_t Write(const std::vector<System::t::Byte> Buffer, int32_t Offset, int32_t Count) { throw(System::Exception(L"Write read-only stream")); }
	virtual int32_t Seek(int32_t Offset, System::Word Origin);
	virtual int64_t Seek(const int64_t Offset, TSeekOrigin Origin) override;
};


#endif /* SRC_CTOOL1CD_TABLEFILESTREAM_H_ */
