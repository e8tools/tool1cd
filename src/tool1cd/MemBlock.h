/*
 * MemBlock.h
 *
 *  Created on: 24 июн. 2017 г.
 *
 */

#ifndef SRC_CTOOL1CD_MEMBLOCK_H_
#define SRC_CTOOL1CD_MEMBLOCK_H_

#include <vector>
#include "Common.h"

class MemBlock
{
public:
	MemBlock(uint32_t page_size, uint32_t block_num)
			: block_num(block_num), last_data_get(0)
	{
		data.resize(page_size);
	}

	char *getblock(bool for_write);

	const char *getblock() const;

	void write(std::shared_ptr<TFileStream> &fs);

	uint32_t get_last_data_get() const;
	bool is_changed() const;

private:
	std::vector<char> data;
	uint32_t block_num {0};
	mutable uint32_t last_data_get {0};
	bool changed {false};

public:
	static std::shared_ptr<MemBlock> create(
			std::shared_ptr<TFileStream> &fs,
			uint32_t page_size,
			uint32_t block_num,
			bool for_write,
			bool read);
};

// класс кешированного блока в памяти
// первый в цепочке кешированный блок - тот, к которому наиболее давно обращались
// последний в цепочке - с самым последним обращением
class MemBlockManager
{
public:
	MemBlockManager();
	explicit MemBlockManager(std::shared_ptr<TFileStream> &fs);
	~MemBlockManager() = default;

	uint32_t count; // текущее количество кешированных блоков

	void garbage(bool aggressive);
	char* getblock(uint32_t _numblock);
	char* getblock_for_write(uint32_t _numblock, bool read);
	void create_memblocks(uint64_t _numblocks);

	void delete_memblocks();
	uint64_t get_numblocks() const;
	void flush();

	uint32_t get_page_size() const;
	void set_page_size(const uint32_t value);

	uint32_t get_maxcount() const;
	void set_maxcount(const uint32_t value);

private:
	uint32_t pagesize; // размер одной страницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	std::shared_ptr<TFileStream> fs; // файл, которому принадлежит блок

	uint32_t maxcount; // максимальное количество кешированных блоков
	// TODO: тут должны быть уник_птр
	std::vector<std::shared_ptr<MemBlock>> memblocks; // указатель на массив указателей MemBlockManager (количество равно количеству блоков в файле *.1CD)

	// получить блок для чтения или для записи
	void add_block();

};

#endif /* SRC_CTOOL1CD_MEMBLOCK_H_ */
