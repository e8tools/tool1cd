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


#ifndef SRC_CTOOL1CD_MEMBLOCK_H_
#define SRC_CTOOL1CD_MEMBLOCK_H_

#include <vector>
#include "Common.h"
#include "SystemClasses/TFileStream.hpp"

class MemBlock
{
public:
	MemBlock(uint32_t page_size, uint32_t block_num)
			: block_num(block_num), last_data_get(0)
	{
		data.resize(page_size);
	}

	char *get_block(bool for_write);

	const char *get_block() const;

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
	char* get_block(uint32_t _numblock);
	char* get_block_for_write(uint32_t _numblock, bool read);
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
