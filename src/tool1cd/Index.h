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

#ifndef SRC_CTOOL1CD_INDEX_H_
#define SRC_CTOOL1CD_INDEX_H_

#include <vector>

#include "Common.h"
#include "Class_1CD.h"
#include "Field.h"
#include "V8Object.h"
#include "db_ver.h"

class V8Object;
class Field;
class TableRecord;

struct IndexRecord
{
	Field* field {nullptr};
	int32_t len {0};
};

#pragma pack(push)
#pragma pack(1)

// структура одной записи распакованного индекса страницы-листа
struct UnpackIndexRecord
{
	uint32_t _record_number; // номер (индекс) записи в таблице записей
	unsigned char _index[1]; // значение индекса записи. Реальная длина значения определяется полем length класса index
};

// структура заголовка страницы-ветки индексов
struct BranchPageHeader {
	uint16_t flags; // offset 0
	uint16_t number_indexes; // offset 2
	uint32_t prev_page; // offset 4 // для 8.3.8 - это номер страницы (реальное смещение = prev_page * pagesize), до 8.3.8 - это реальное смещение
	uint32_t next_page; // offset 8 // для 8.3.8 - это номер страницы (реальное смещение = next_page * pagesize), до 8.3.8 - это реальное смещение

	static constexpr uint32_t Size() {
		return sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t);
	}
};

// структура заголовка страницы-листа индексов
struct LeafPageHeader {
	int16_t flags; // offset 0
	uint16_t number_indexes; // offset 2
	uint32_t prev_page; // offset 4 // для 8.3.8 - это номер страницы (реальное смещение = prev_page * pagesize), до 8.3.8 - это реальное смещение
	uint32_t next_page; // offset 8 // для 8.3.8 - это номер страницы (реальное смещение = next_page * pagesize), до 8.3.8 - это реальное смещение
	uint16_t freebytes; // offset 12
	uint32_t numrecmask; // offset 14
	uint16_t leftmask; // offset 18
	uint16_t rightmask; // offset 20
	uint16_t numrecbits; // offset 22
	uint16_t leftbits; // offset 24
	uint16_t rightbits; // offset 26
	uint16_t recbytes; // offset 28
};

// Вспомогательная структура для упаковки индексов на странице-листе
struct _pack_index_record
{
	uint32_t numrec;
	uint32_t left;
	uint32_t right;
};

#pragma pack(pop)

// Значения битовых флагов в заголовке страницы индекса
const int16_t indexpage_is_root = 1; // Установленный флаг означает, что страница является корневой
const int16_t indexpage_is_leaf = 2; // Установленный флаг означает, что страница является листом, иначе веткой

class Index
{
public:
	explicit Index(Table* _base);
	~Index();

	std::string get_name() const;
	size_t get_num_records() const; // получить количество полей в индексе
	std::vector<IndexRecord>& get_records();

	uint32_t get_numrecords() const; // получает количество записей, проиндексированных индексом
	uint32_t get_numrec(uint32_t num_record) const; // получает физический индекс записи по порядковому индексу

	void dump(const std::string &filename);
	void calcRecordIndex(const TableRecord *rec, char *indexBuf); // вычислить индекс записи rec и поместить в indexBuf. Длина буфера indexBuf должна быть не меньше length

	uint32_t get_rootblock() const;
	uint32_t get_length() const;

	uint64_t get_start_offset() const;
	void set_start_offset(const uint64_t value);

	// распаковывает одну страницу-лист индексов
	// возвращает массив структур unpack_index_record. Количество элементов массива возвращается в number_indexes
	char* unpack_leafpage(uint64_t page_offset, uint32_t& number_indexes);

	// распаковывает одну страницу-лист индексов
	// возвращает массив структур unpack_index_record. Количество элементов массива возвращается в number_indexes
	char* unpack_leafpage(char* page, uint32_t& number_indexes);

	// упаковывает одну страницу-лист индексов.
	// возвращает истина, если упаковка произведена, и ложь, если упаковка невозможна.
	bool pack_leafpage(char* unpack_index, uint32_t number_indexes, char* page_buf);

	bool is_primary() const;

	static Index *index_from_tree(Tree *f, Table *parent);

	void write_index(const uint32_t phys_numrecord, const TableRecord *rec); // запись индекса записи
	void delete_index(const TableRecord *rec, const uint32_t phys_numrec); // удаление индекса записи из файла index

private:
	Table* tbase;
	db_ver version; // версия базы
	uint32_t pagesize; // размер одной страницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))

	std::string name;
	bool primary {false};

	std::vector<IndexRecord> records;

	uint64_t start {0}; // Смещение в файле индексов блока описания индекса
	// TODO: убрать mutable
	mutable uint64_t rootblock; // Смещение в файле индексов корневого блока индекса

	// TODO: убрать mutable

	mutable uint32_t length; // длина в байтах одной распакованной записи индекса
	mutable std::vector<uint32_t> recordsindex; // динамический массив индексов записей по номеру (только не пустые записи)
	mutable bool recordsindex_complete; // признак заполнености recordsindex
	void create_recordsindex() const;

	void dump_recursive(V8Object* file_index, TFileStream* f, int32_t level, uint64_t curblock);
	void delete_index_record(const char* index_buf, const uint32_t phys_numrec); // удаление одного индекса из файла index
	void delete_index_record(const char* index_buf, const uint32_t phys_numrec, uint64_t block, bool& is_last_record, bool& page_is_empty, char* new_last_index_buf, uint32_t& new_last_phys_num); // рекурсивное удаление одного индекса из блока файла index
	void write_index_record(const uint32_t phys_numrecord, const char* index_buf); // запись индекса
	void write_index_record(const uint32_t phys_numrecord, const char* index_buf, uint64_t block, int32_t& result, char* new_last_index_buf, uint32_t& new_last_phys_num, char* new_last_index_buf2, uint32_t& new_last_phys_num2, uint64_t& new_last_block2); // рекурсивная запись индекса

};

#endif /* SRC_CTOOL1CD_INDEX_H_ */
