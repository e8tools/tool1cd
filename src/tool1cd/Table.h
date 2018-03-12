/*
 *
 *
 *
 *
 */

#ifndef SRC_CTOOL1CD_TABLE_H_
#define SRC_CTOOL1CD_TABLE_H_

#include "Common.h"
#include "V8Object.h"
#include "Field.h"
#include "Index.h"
#include "Class_1CD.h"
#include "TableRecord.h"

static const uint32_t BLOB_RECORD_LEN = 256;
static const uint32_t BLOB_RECORD_DATA_LEN = 250;

class Index;

enum table_info
{
	ti_description,
	ti_fields,
	ti_indexes,
	ti_physical_view,
	ti_logical_view
};

// типы измененных записей
enum class changed_rec_type
{
	not_changed,
	changed,
	inserted,
	deleted
};

// структура изменной записи таблицы
class changed_rec
{
public:
	// владелец
	Table* parent;

	// физический номер записи (для добавленных записей нумерация начинается с phys_numrecords)
	uint32_t numrec;

	// тип изменения записи (изменена, добавлена, удалена)
	changed_rec_type changed_type;

	// следующая измененная запись в списке измененных записей
	changed_rec* next;

	// массив признаков изменения поля (по одному байту на каждое поле, всего num_fields байт)
	char* fields;

	// измененная запись. Для типов полей tf_text (TEXT), tf_string (MEMO) и tf_image (BLOB), если соответствующий признак в fields установлен,
	// содержит указатель на TStream с содержимым поля (или NULL)
	char* rec;

	changed_rec(Table* _parent, changed_rec_type crt, uint32_t phys_numrecord);
	~changed_rec();
	void clear();
};

// структура одного блока в файле file_blob
struct blob_block{
	uint32_t nextblock;
	int16_t length;
	char data[BLOB_RECORD_DATA_LEN];
};

// структура root файла экспорта/импорта таблиц
struct export_import_table_root
{
	bool has_data;
	bool has_blob;
	bool has_index;
	bool has_descr;
	int32_t data_version_1; // версия реструктуризации
	int32_t data_version_2; // версия изменения
	int32_t blob_version_1; // версия реструктуризации
	int32_t blob_version_2; // версия изменения
	int32_t index_version_1; // версия реструктуризации
	int32_t index_version_2; // версия изменения
	int32_t descr_version_1; // версия реструктуризации
	int32_t descr_version_2; // версия изменения
};

class Table{
friend Field;
friend Index;
friend changed_rec;
friend T_1CD;
public:
	//--> поддержка динамического построения таблицы записей
	std::vector<uint32_t> recordsindex; // массив индексов записей по номеру (только не пустые записи)
	bool recordsindex_complete; // признак заполнености recordsindex
	uint32_t numrecords_review; // количество просмотренных записей всего в поиске не пустых
	uint32_t numrecords_found; // количество найденных непустых записей (текущий размер recordsindex)
	//<-- поддержка динамического построения таблицы записей
	void fillrecordsindex(); // заполнить recordsindex не динамически


	Table();
	Table(T_1CD* _base, int32_t block_descr);
	Table(T_1CD *_base, const std::string &_descr, int32_t block_descr = 0);
	~Table();
	void init();
	void init(int32_t block_descr);

	std::string getname() const;
	std::string getdescription() const;
	int32_t get_numfields() const;
	int32_t get_numindexes() const;
	Field* getfield(int32_t numfield);
	Index* getindex(int32_t numindex);
	bool get_issystem() const;
	int32_t get_recordlen() const;
	bool get_recordlock() const;

	uint32_t get_phys_numrecords() const; // возвращает количество записей в таблице всего, вместе с удаленными
	uint32_t get_log_numrecords() const; // возвращает количество записей в таблице всего, без удаленных
	void set_log_numrecords(uint32_t _log_numrecords); //
	uint32_t get_added_numrecords() const;

	TableRecord *getrecord(uint32_t phys_numrecord) const; // возвращает указатель на запись, буфер принадлежит вызывающей процедуре
	void getrecord(uint32_t phys_numrecord, char *buf);
	TStream* readBlob(TStream* _str, uint32_t _startblock, uint32_t _length, bool rewrite = true) const;
	uint32_t readBlob(void* _buf, uint32_t _startblock, uint32_t _length) const;
	void set_lockinmemory(bool _lock);
	bool export_to_xml(const std::string &filename, bool blob_to_file, bool unpack) const;

	V8Object* get_file_data();
	V8Object* get_file_blob();
	V8Object* get_file_index();

	uint64_t get_fileoffset(uint32_t phys_numrecord); // получить физическое смещение в файле записи по номеру

	TableRecord *get_edit_record(uint32_t phys_numrecord); // возвращает указатель на запись, буфер принадлежит вызывающей процедуре
	bool get_edit() const;

	uint32_t get_phys_numrec(int32_t ARow, Index* cur_index); // получить физический индекс записи по номеру строки по указанному индексу
	std::string get_file_name_for_field(int32_t num_field, char *rec, uint32_t numrec = 0); // получить имя файла по-умолчанию конкретного поля конкретной записи
	std::string get_file_name_for_record(const TableRecord *rec) const; // получить имя файла по-умолчанию конкретной записи
	T_1CD* getbase(){return base;}

	void begin_edit(); // переводит таблицу в режим редактирования
	void cancel_edit(); // переводит таблицу в режим просмотра и отменяет все изменения
	void end_edit(); // переводит таблицу в режим просмотра и сохраняет все изменения
	changed_rec_type get_rec_type(uint32_t phys_numrecord);
	changed_rec_type get_rec_type(uint32_t phys_numrecord, int32_t numfield);
	void set_edit_value(uint32_t phys_numrecord, int32_t numfield, bool null, const std::string &value,
						TStream *st = nullptr);
	void restore_edit_value(uint32_t phys_numrecord, int32_t numfield);
	void set_rec_type(uint32_t phys_numrecord, changed_rec_type crt);

	void export_table(const std::string &path) const;
	void import_table(const std::string &path);

	void delete_record(uint32_t phys_numrecord); // удаление записи
	void insert_record(const TableRecord *rec); // добавление записи
	void update_record(uint32_t phys_numrecord, char* rec, char* changed_fields); // изменение записи
	char* get_record_template_test();

	Field* get_field(const std::string &fieldname) const;
	Field* find_field(const std::string &fieldname) const throw();
	Index* get_index(const std::string &indexname) const;
	Index* find_index(const std::string &indexname) const throw();

private:
	T_1CD* base;

	V8Object* descr_table; // объект с описанием структуры таблицы (только для версий с 8.0 до 8.2.14)
	std::string description;
	std::string name;
	int32_t num_fields;
	std::vector<Field*> fields;
	int32_t num_indexes;
	std::vector<Index*> indexes;
	bool recordlock;
	V8Object* file_data;
	V8Object* file_blob;
	V8Object* file_index;
	int32_t recordlen; // длина записи (в байтах)
	bool issystem; // Признак системной таблицы (имя таблицы не начинается с подчеркивания)
	int32_t lockinmemory; // счетчик блокировок в памяти

	void deletefields();
	void deleteindexes();

	changed_rec* ch_rec; // первая измененная запись в списке измененных записей
	uint32_t added_numrecords; // количество добавленных записей в режиме редактирования

	uint32_t phys_numrecords; // физическое количество записей (вместе с удаленными)
	uint32_t log_numrecords; // логическое количество записей (только не удаленные)

	void create_file_data(); // создание файла file_data
	void create_file_blob(); // создание файла file_blob
	void create_file_index(); // создание файла file_index
	void refresh_descr_table(); // создание и запись файла описания таблицы

	bool edit; // признак, что таблица находится в режиме редактирования

	void delete_data_record(uint32_t phys_numrecord); // удаление записи из файла data
	void delete_blob_record(uint32_t blob_numrecord); // удаление записи из файла blob
	void delete_index_record(uint32_t phys_numrecord); // удаление всех индексов записи из файла index
	void delete_index_record(uint32_t phys_numrecord, const TableRecord *rec); // удаление всех индексов записи из файла index
	void write_data_record(uint32_t phys_numrecord, const TableRecord *rec); // запись одной записи в файл data
	uint32_t write_blob_record(char* blob_record, uint32_t blob_len); // записывает НОВУЮ запись в файл blob, возвращает индекс новой записи
	uint32_t write_blob_record(TStream* bstr); //  // записывает НОВУЮ запись в файл blob, возвращает индекс новой записи
	void write_index_record(const uint32_t phys_numrecord, const TableRecord *rec); // запись индексов записи в файл index

	bool bad; // признак битой таблицы
};


#endif /* SRC_CTOOL1CD_TABLE_H_ */
