/*
 * MemBlock.h
 *
 *  Created on: 24 июн. 2017 г.
 *
 */

#ifndef SRC_CTOOL1CD_MEMBLOCK_H_
#define SRC_CTOOL1CD_MEMBLOCK_H_

#include "Common.h"
#include "Class_1CD.h"

class T_1CD;

// класс кешированного блока в памяти
// первый в цепочке кешированный блок - тот, к которому наиболее давно обращались
// последний в цепочке - с самым последним обращением
class MemBlock
{
public:
	static uint32_t count; // текущее количество кешированных блоков

	static void garbage();
	static char* getblock(TFileStream* fs, uint32_t _numblock);
	static char* getblock_for_write(TFileStream* fs, uint32_t _numblock, bool read);
	static void create_memblocks(uint64_t _numblocks);

	static void delete_memblocks();
	static uint64_t get_numblocks();
	static void flush();

	static uint32_t get_page_size();
	static void set_page_size(const uint32_t value);

	static uint32_t get_maxcount();
	static void set_maxcount(const uint32_t value);

private:
	char* buf; // указатель на блок в памяти
	static uint32_t pagesize; // размер одной страницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	uint32_t numblock;
	MemBlock* next;
	MemBlock* prev;
	TFileStream* file; // файл, которому принадлежит блок
	bool is_changed; // признак, что блок изменен (требует записи)

	static MemBlock* first;
	static MemBlock* last;
	static uint32_t maxcount; // максимальное количество кешированных блоков
	static uint64_t numblocks;   // количество значащих элементов в массиве memblocks (равно количеству блоков в файле *.1CD)

	static uint64_t array_numblocks;   // количество элементов в массиве memblocks (больше или равно количеству блоков в файле *.1CD)
	static uint32_t delta; // шаг увеличения массива memblocks
	static MemBlock** memblocks; // указатель на массив указателей MemBlock (количество равно количеству блоков в файле *.1CD)

	uint32_t lastdataget; // время (Windows time, в миллисекундах) последнего обращения к данным объекта (data)
	char* getblock(bool for_write); // получить блок для чтения или для записи
	MemBlock(TFileStream* fs, uint32_t _numblock, bool for_write, bool read);
	~MemBlock();
	static void add_block();
	void write();

};

#endif /* SRC_CTOOL1CD_MEMBLOCK_H_ */
