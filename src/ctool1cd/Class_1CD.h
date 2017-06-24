//---------------------------------------------------------------------------

#ifndef Class_1CDH
#define Class_1CDH

#include "vector"

#include "MessageRegistration.h"
#include "APIcfBase.h"
#include "db_ver.h"
#include "Parse_tree.h"
#include "V8Object.h"

//---------------------------------------------------------------------------

class T_1CD;
class Table;
class v8object;

class ConfigStorageTableConfig;
class ConfigStorageTableConfigSave;

#pragma pack(push)
#pragma pack(1)

// структура первой страницы контейнера
struct v8con
{
	char sig[8]; // сигнатура SIG_CON
	char ver1;
	char ver2;
	char ver3;
	char ver4;
	uint32_t length;
	int32_t firstblock;
	uint32_t pagesize;

	String getver(){
		String ss = (int32_t)ver1;
		ss += ".";
		ss += (int32_t)ver2;
		ss += ".";
		ss += (int32_t)ver3;
		ss += ".";
		ss += (int32_t)ver4;
		return ss;
	}
};

// Структура страницы размещения уровня 1 версий от 8.0 до 8.2.14
struct objtab
{
	int32_t numblocks;
	uint32_t blocks[1023];
};

#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)

struct root_80
{
	char lang[8];
	int32_t numblocks;
	uint32_t blocks[1];
};

struct root_81
{
	char lang[32];
	int32_t numblocks;
	uint32_t blocks[1];
};

#pragma pack(pop)

union root
{
	root_80 root80;
	root_81 root81;
};

enum type_fields
{
	tf_binary, // B // длина = length
	tf_bool, // L // длина = 1
	tf_numeric, // N // длина = (length + 2) / 2
	tf_char, // NC // длина = length * 2
	tf_varchar, // NVC // длина = length * 2 + 2
	tf_version, // RV // 16, 8 версия создания и 8 версия модификации ? каждая версия int32_t(изменения) + int32_t(реструктуризация)
	tf_string, // NT // 8 (unicode text)
	tf_text, // T // 8 (ascii text)
	tf_image, // I // 8 (image = bynary data)
	tf_datetime, // DT //7
	tf_version8, // 8, скрытое поле при recordlock == false и отсутствии поля типа tf_version
	tf_varbinary // VB // длина = length + 2
};

// Стили преобразования bynary16 в GUID
//
// Исходное значение
// 00112233445566778899aabbccddeeff
//
// 1С style
// ccddeeff-aabb-8899-0011-223344556677
//
// MS style
// 33221100-5544-7766-8899-aabbccddeeff
//


class Field
{
friend Table;
friend T_1CD;
public:
	static bool showGUID;

	Field(Table* _parent);

	int32_t getlen(); // возвращает длину поля в байтах
	String getname();
	String get_presentation(const char* rec, bool EmptyNull = false, wchar_t Delimiter = 0, bool ignore_showGUID = false, bool detailed = false);
	String get_XML_presentation(char* rec, bool ignore_showGUID = false);
	bool get_bynary_value(char* buf, bool null, String& value);
	type_fields gettype();
	Table* getparent();
	bool getnull_exists();
	int32_t getlength();
	int32_t getprecision();
	bool getcase_sensitive();
	int32_t getoffset();
	String get_presentation_type();
	bool save_blob_to_file(char* rec, String filename, bool unpack);
#ifndef PublicRelease
	uint32_t getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen);
#endif //#ifdef PublicRelease
private:
	String name;
	type_fields type;
	bool null_exists;
	int32_t length;
	int32_t precision;
	bool case_sensitive;

	Table* parent;
	int32_t len; // длина поля в байтах
	int32_t offset; // смещение поля в записи
	static bool showGUIDasMS; // Признак, что GUID надо преобразовывать по стилю MS (иначе по стилю 1С)
	static char buf[];
	static char null_index[];
	static bool null_index_initialized;
};

struct index_record
{
	Field* field;
	int32_t len;
};

#pragma pack(push)
#pragma pack(1)


// структура одной записи распакованного индекса страницы-листа
struct unpack_index_record
{
	uint32_t _record_number; // номер (индекс) записи в таблице записей
	unsigned char _index[1]; // значение индекса записи. Реальная длина значения определяется полем length класса index
};

// структура заголовка страницы-ветки индексов
struct branch_page_header{
	uint16_t flags; // offset 0
	uint16_t number_indexes; // offset 2
	uint32_t prev_page; // offset 4 // для 8.3.8 - это номер страницы (реальное смещение = prev_page * pagesize), до 8.3.8 - это реальное смещение
	uint32_t next_page; // offset 8 // для 8.3.8 - это номер страницы (реальное смещение = next_page * pagesize), до 8.3.8 - это реальное смещение
};

// структура заголовка страницы-листа индексов
struct leaf_page_header{
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

// Значения битовых флагов в заголовке стрницы индекса
const int16_t indexpage_is_root = 1; // Установленный флаг означает, что страница является корневой
const int16_t indexpage_is_leaf = 2; // Установленный флаг означает, что страница является листом, иначе веткой

class index
{
friend Table;
private:
	Table* tbase;
	db_ver version; // версия базы
	uint32_t pagesize; // размер одной стрницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))

	String name;
	bool is_primary;
	int32_t num_records; // количество полей в индексе
	index_record* records;

	uint64_t start; // Смещение в файле индексов блока описания индекса
	uint64_t rootblock; // Смещение в файле индексов корневого блока индекса
	uint32_t length; // длина в байтах одной распакованной записи индекса
	DynamicArray<uint32_t> recordsindex; // динамический массив индексов записей по номеру (только не пустые записи)
	bool recordsindex_complete; // признак заполнености recordsindex
	void create_recordsindex();

#ifndef PublicRelease
	void dump_recursive(v8object* file_index, TFileStream* f, int32_t level, uint64_t curblock);
	void delete_index(const char* rec, const uint32_t phys_numrec); // удаление индекса записи из файла index
	void delete_index_record(const char* index_buf, const uint32_t phys_numrec); // удаление одного индекса из файла index
	void delete_index_record(const char* index_buf, const uint32_t phys_numrec, uint64_t block, bool& is_last_record, bool& page_is_empty, char* new_last_index_buf, uint32_t& new_last_phys_num); // рекурсивное удаление одного индекса из блока файла index
	void write_index(const uint32_t phys_numrecord, const char* rec); // запись индекса записи
	void write_index_record(const uint32_t phys_numrecord, const char* index_buf); // запись индекса
	void write_index_record(const uint32_t phys_numrecord, const char* index_buf, uint64_t block, int32_t& result, char* new_last_index_buf, uint32_t& new_last_phys_num, char* new_last_index_buf2, uint32_t& new_last_phys_num2, uint64_t& new_last_block2); // рекурсивная запись индекса
#endif //#ifdef PublicRelease

public:
	index(Table* _base);
	~index();

	String getname();
	bool get_is_primary();
	int32_t get_num_records(); // получить количество полей в индексе
	index_record* get_records();

	uint32_t get_numrecords(); // получает количество записей, проиндексированных индексом
	uint32_t get_numrec(uint32_t num_record); // получает физический индекс записи по порядковому индексу

#ifndef PublicRelease
	void dump(String filename);
	void calcRecordIndex(const char* rec, char* indexBuf); // вычислить индекс записи rec и поместить в indexBuf. Длина буфера indexBuf должна быть не меньше length
#endif //#ifdef PublicRelease

	uint32_t get_rootblock();
	uint32_t get_length();

	// распаковывает одну страницу-лист индексов
	// возвращает массив структур unpack_index_record. Количество элементов массива возвращается в number_indexes
	char* unpack_leafpage(uint64_t page_offset, uint32_t& number_indexes);

	// распаковывает одну страницу-лист индексов
	// возвращает массив структур unpack_index_record. Количество элементов массива возвращается в number_indexes
	char* unpack_leafpage(char* page, uint32_t& number_indexes);

	// упаковывает одну страницу-лист индексов.
	// возвращвет истина, если упаковка произведена, и ложь, если упаковка невозможна.
	bool pack_leafpage(char* unpack_index, uint32_t number_indexes, char* page_buf);

};

enum table_info
{
	ti_description,
	ti_fields,
	ti_indexes,
	ti_physical_view,
	ti_logical_view
};

// типы измененных записей
enum changed_rec_type
{
	crt_not_changed,
	crt_changed,
	crt_insert,
	crt_delete
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
	char data[250];
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
friend class index;
friend changed_rec;
friend T_1CD;
private:
	T_1CD* base;

	v8object* descr_table; // объект с описанием структуры таблицы (только для версий с 8.0 до 8.2.14)
	String description;
	String name;
	int32_t num_fields;
	int32_t num_fields2; // количество элементов в массиве fields
	Field** fields;
	int32_t num_indexes;
	class index** indexes;
	bool recordlock;
	v8object* file_data;
	v8object* file_blob;
	v8object* file_index;
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
#ifndef PublicRelease

	bool edit; // признак, что таблица находится в режиме редактирования

	void delete_data_record(uint32_t phys_numrecord); // удаление записи из файла data
	void delete_blob_record(uint32_t blob_numrecord); // удаление записи из файла blob
	void delete_index_record(uint32_t phys_numrecord); // удаление всех индексов записи из файла index
	void delete_index_record(uint32_t phys_numrecord, char* rec); // удаление всех индексов записи из файла index
	void write_data_record(uint32_t phys_numrecord, char* rec); // запись одной записи в файл data
	uint32_t write_blob_record(char* blob_record, uint32_t blob_len); // записывает НОВУЮ запись в файл blob, возвращает индекс новой записи
	uint32_t write_blob_record(TStream* bstr); //  // записывает НОВУЮ запись в файл blob, возвращает индекс новой записи
	void write_index_record(const uint32_t phys_numrecord, const char* rec); // запись индексов записи в файл index
#endif //#ifdef PublicRelease

	bool bad; // признак битой таблицы

public:
	//--> поддержка динамического построения таблицы записей
	uint32_t* recordsindex; // массив индексов записей по номеру (только не пустые записи)
	bool recordsindex_complete; // признак заполнености recordsindex
	uint32_t numrecords_review; // количество просмотренных записей всего в поиске не пустых
	uint32_t numrecords_found; // количество найденных непустых записей (текущий размер recordsindex)
	//<-- поддержка динамического построения таблицы записей
	void fillrecordsindex(); // заполнить recordsindex не динамически


	Table();
	Table(T_1CD* _base, int32_t block_descr);
	Table(T_1CD* _base, String _descr, int32_t block_descr = 0);
	~Table();
	void init(int32_t block_descr = 0);

	String getname();
	String getdescription();
	int32_t get_numfields();
	int32_t get_numindexes();
	Field* getfield(int32_t numfield);
	class index* getindex(int32_t numindex);
	bool get_issystem();
	int32_t get_recordlen();
	bool get_recordlock();

	uint32_t get_phys_numrecords(); // возвращает количество записей в таблице всего, вместе с удаленными
	uint32_t get_log_numrecords(); // возвращает количество записей в таблице всего, без удаленных
	void set_log_numrecords(uint32_t _log_numrecords); //
	uint32_t get_added_numrecords();

	char* getrecord(uint32_t phys_numrecord, char* buf); // возвращает указатель на запись, буфер принадлежит вызывающей процедуре
	TStream* readBlob(TStream* _str, uint32_t _startblock, uint32_t _length, bool rewrite = true);
	uint32_t readBlob(void* _buf, uint32_t _startblock, uint32_t _length);
	void set_lockinmemory(bool _lock);
	bool export_to_xml(String filename, bool blob_to_file, bool unpack);

	v8object* get_file_data();
	v8object* get_file_blob();
	v8object* get_file_index();

	int64_t get_fileoffset(uint32_t phys_numrecord); // получить физическое смещение в файле записи по номеру

	char* get_edit_record(uint32_t phys_numrecord, char* buf); // возвращает указатель на запись, буфер принадлежит вызывающей процедуре
	bool get_edit();

	uint32_t get_phys_numrec(int32_t ARow, class index* cur_index); // получить физический индекс записи по номеру строки по указанному индексу
	String get_file_name_for_field(int32_t num_field, char* rec, uint32_t numrec = 0); // получить имя файла по-умолчанию конкретного поля конкретной записи
	String get_file_name_for_record(char* rec); // получить имя файла по-умолчанию конкретной записи
	T_1CD* getbase(){return base;};

#ifndef PublicRelease
	void begin_edit(); // переводит таблицу в режим редактирования
	void cancel_edit(); // переводит таблицу в режим просмотра и отменяет все изменения
	void end_edit(); // переводит таблицу в режим просмотра и сохраняет все изменения
	changed_rec_type get_rec_type(uint32_t phys_numrecord);
	changed_rec_type get_rec_type(uint32_t phys_numrecord, int32_t numfield);
	void set_edit_value(uint32_t phys_numrecord, int32_t numfield, bool null, String value, TStream* st = NULL);
	void restore_edit_value(uint32_t phys_numrecord, int32_t numfield);
	void set_rec_type(uint32_t phys_numrecord, changed_rec_type crt);

	void export_table(const String &path) const;
	void import_table(const String &path);

	void delete_record(uint32_t phys_numrecord); // удаление записи
	void insert_record(char* rec); // добавление записи
	void update_record(uint32_t phys_numrecord, char* rec, char* changed_fields); // изменение записи
	char* get_record_template_test();
#endif //#ifdef PublicRelease

};

//---------------------------------------------------------------------------
// Структура адреса файла таблицы-контейнера файлов
struct table_blob_file
{
	uint32_t blob_start;
	uint32_t blob_length;
};

//---------------------------------------------------------------------------
// Структура записи таблицы контейнера файлов
struct table_rec
{
	String name;
	table_blob_file addr;
	int32_t partno;
	FILETIME ft_create;
	FILETIME ft_modify;
};

//---------------------------------------------------------------------------
// Структура файла таблицы контейнера файлов
struct table_file
{
	Table* t;
	String name; // Имя, как оно хранится в таблице
	uint32_t maxpartno;
	table_blob_file* addr;
	FILETIME ft_create;
	FILETIME ft_modify;

	table_file(Table* _t, const String& _name, uint32_t _maxpartno);
	~table_file();
};

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFiles
{
private:
	Table* tab;
	std::map<String,table_file*> allfiles;
	char* rec;
	bool ready;

	bool test_table();
public:
	TableFiles(Table* t);
	virtual ~TableFiles();
	bool getready(){return ready;};
	table_file* getfile(const String& name);
	Table* gettable(){return tab;};

	// __property std::map<String,table_file*> files = {read = allfiles};
	std::map<String,table_file*> &files();
};

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
	virtual int32_t Write(const void *Buffer, int32_t Count){throw(Exception(L"Write read-only stream"));};
	virtual int32_t Write(const System::DynamicArray<System::t::Byte> Buffer, int32_t Offset, int32_t Count){throw(Exception(L"Write read-only stream"));};
	virtual int32_t Seek(int32_t Offset, System::Word Origin);
	virtual int64_t Seek(const int64_t Offset, TSeekOrigin Origin);
};


// класс кешированного блока в памяти
// первый в цепочке кешированный блок - тот, к которому наиболее давно обращались
// последний в цепочке - с самым последним обращением
class memblock
{
friend T_1CD;
private:
	char* buf; // указатель на блок в памяти
	static uint32_t pagesize; // размер одной стрницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	uint32_t numblock;
	memblock* next;
	memblock* prev;
	TFileStream* file; // файл, которому принадлежит блок
	bool is_changed; // признак, что блок изменен (требует записи)

	static memblock* first;
	static memblock* last;
	static uint32_t maxcount; // максимальное количество кешированных блоков
	static uint64_t numblocks;   // количество значащих элементов в массиве memblocks (равно количеству блоков в файле *.1CD)

	static uint64_t array_numblocks;   // количество элементов в массиве memblocks (больше или равно количеству блоков в файле *.1CD)
	static uint32_t delta; // шаг увеличения массива memblocks
	static memblock** memblocks; // указатель на массив указателей memblock (количество равно количеству блоков в файле *.1CD)

	uint32_t lastdataget; // время (Windows time, в миллисекундах) последнего обращения к данным объекта (data)
	char* getblock(bool for_write); // получить блок для чтения или для записи
	memblock(TFileStream* fs, uint32_t _numblock, bool for_write, bool read);
	~memblock();
	static void add_block();
	void write();

public:
	static uint32_t count; // текущее количество кешированных блоков

	static void garbage();
	static char* getblock(TFileStream* fs, uint32_t _numblock);
	static char* getblock_for_write(TFileStream* fs, uint32_t _numblock, bool read);
	static void create_memblocks(uint64_t _numblocks);

	static void delete_memblocks();
	static uint64_t get_numblocks();
	static void flush();
};

// класс конфигурации поставщика
class SupplierConfig
{
public:
	table_file* file;
	String name; // имя конфигурация поставщика
	String supplier; // синоним конфигурация поставщика
	String version; // версия конфигурация поставщика
};

// Структура одной записи индекса упакованных объектов хранилища 8.3 (из файла *.ind)

#pragma pack(push)
#pragma pack(1)
struct _datahash
{
	char datahash[20]; // хэш файла
	int64_t offset; // смещение файла в файле *.pck
};
#pragma pack(pop)

// Структура для одной пары файлов *.ind и *.pck в каталоге data\pack\ хранилища 8.3
struct _packdata
{
	TFileStream* pack; // открытый на чтение файл *.pck
	uint32_t count; // кол-во записей (объектов) в файле *.pck
	_datahash* datahashes; // массив
};

// Типы страниц
enum pagetype
{
	pt_lost, // потерянная страница (не относится ни к одному объекту)
	pt_root, // корневая страница (страница 0)
	pt_freeroot, // корневая страница таблицы свободных блоков (страница 1)
	pt_freealloc, // страница размещения таблицы свободных блоков
	pt_free, // свободная страница
	pt_rootfileroot, // корневая страница корневого файла (страница 2)
	pt_rootfilealloc, // страница размещения корневого файла
	pt_rootfile, // страница данных корневого файла
	pt_descrroot, // корневая страница файла descr таблицы
	pt_descralloc, // страница размещения файла descr таблицы
	pt_descr, // страница данных файла descr таблицы
	pt_dataroot, // корневая страница файла data таблицы
	pt_dataalloc, // страница размещения файла data таблицы
	pt_data, // страница данных файла data таблицы
	pt_indexroot, // корневая страница файла index таблицы
	pt_indexalloc, // страница размещения файла index таблицы
	pt_index, // страница данных файла index таблицы
	pt_blobroot, // корневая страница файла blob таблицы
	pt_bloballoc, // страница размещения файла blob таблицы
	pt_blob // страница данных файла blob таблицы
};

// Структура принадлежности страницы
struct pagemaprec
{
	int32_t tab; // Индекс в T_1CD::tables, -1 - страница не относится к таблицам
	pagetype type; // тип страницы
	uint32_t number; // номер страницы в своем типе
	pagemaprec(){tab = -1; type = pt_lost; number = 0;};
};


// класс базы 1CD
class T_1CD
{
friend v8object;
friend Table;
friend class index;
friend Field;
public:
	static bool recoveryMode;
	char* locale; // код языка базы
	bool is_infobase; // признак информационной базы
	bool is_depot; // признак хранилища конфигурации

	// Таблицы информационной базы
	Table* table_config;
	Table* table_configsave;
	Table* table_params;
	Table* table_files;
	Table* table_dbschema;
	Table* table_configcas;
	Table* table_configcassave;
	Table* table__extensionsinfo;

	// таблицы - хранилища файлов
	ConfigStorageTableConfig* cs_config;
	ConfigStorageTableConfigSave* cs_configsave;

	// Таблицы хранилища конфигураций
	Table* table_depot;
	Table* table_users;
	Table* table_objects;
	Table* table_versions;
	Table* table_labels;
	Table* table_history;
	Table* table_lastestversions;
	Table* table_externals;
	Table* table_selfrefs;
	Table* table_outrefs;

	String ver;

	std::vector<SupplierConfig> supplier_configs; // конфигурации поставщика
	bool supplier_configs_defined; // признак, что был произведен поиск конфигураций поставщика

	T_1CD(String _filename, MessageRegistrator* mess = nullptr, bool monopoly = true);
	T_1CD();
	~T_1CD();
	bool is_open();
	int32_t get_numtables();
	Table* gettable(int32_t numtable);
	db_ver getversion();

	bool save_config(String filename);
	bool save_configsave(String filename);
	void find_supplier_configs();
	bool save_supplier_configs(uint32_t numcon, const String& filename);
	bool save_depot_config(const String& _filename, int32_t ver = 0);
	bool save_part_depot_config(const String& _filename, int32_t ver_begin, int32_t ver_end);
	int32_t get_ver_depot_config(int32_t ver); // Получение номера версии конфигурации (0 - последняя, -1 - предпоследняя и т.д.)
	bool save_config_ext(const String& _filename, const TGUID& uid, const String& hashname);
	bool save_config_ext_db(const String& _filename, const String& hashname);

	Field* get_field(Table* tab, String fieldname);
	class index* get_index(Table* tab, String indexname);

	bool get_readonly();
	void set_readonly(bool ro);
	void flush();

	bool test_stream_format();
	bool test_list_of_tables(); // проверка списка таблиц (по DBNames)
#ifndef PublicRelease
	void find_lost_objects();
	void find_and_save_lost_objects();
	bool create_table(String path); // создание таблицы из файлов импорта таблиц
	bool delete_table(Table* tab);
	bool delete_object(v8object* ob);
	bool replaceTREF(String mapfile); // замена значений полей ...TREF во всех таблицах базы
	void find_and_create_lost_tables();
	void restore_DATA_allocation_table(Table* tab);
	bool test_block_by_template(uint32_t testblock, char* tt, uint32_t num, int32_t rlen, int32_t len);
#endif //#ifdef PublicRelease
	String& getfilename(){return filename;};
	uint32_t getpagesize(){return pagesize;};
private:
	Registrator msreg_m;
	String filename;
	TFileStream* fs;

	db_ver version; // версия базы
	uint32_t pagesize; // размер одной страницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	uint32_t length; // длина базы в блоках
	v8object* free_blocks; // свободные блоки
	v8object* root_object; // корневой объект
	int32_t num_tables; // количество таблиц
	Table** tables; // таблицы базы
	bool readonly;
	pagemaprec* pagemap; // Массив длиной length

	TableFiles* _files_config;
	TableFiles* _files_configsave;
	TableFiles* _files_params;
	TableFiles* _files_files;
	TableFiles* _files_configcas;
	TableFiles* _files_configcassave;

	TableFiles* get_files_config();
	TableFiles* get_files_configsave();
	TableFiles* get_files_params();
	TableFiles* get_files_files();
	TableFiles* get_files_configcas();
	TableFiles* get_files_configcassave();

	void init();
	bool getblock(void* buf, uint32_t block_number, int32_t blocklen = -1); // буфер принадлежит вызывающей процедуре
	char*  getblock(uint32_t block_number); // буфер не принадлежит вызывающей стороне (принадлежит memblock)
	char*  getblock_for_write(uint32_t block_number, bool read); // буфер не принадлежит вызывающей стороне (принадлежит memblock)
	void set_block_as_free(uint32_t block_number); // пометить блок как свободный
	uint32_t get_free_block(); // получить номер свободного блока (и пометить как занятый)

	void add_supplier_config(table_file* file);

	bool recursive_test_stream_format(Table* t, uint32_t nrec);
	bool recursive_test_stream_format2(Table* t, uint32_t nrec); // для DBSCHEMA
	bool recursive_test_stream_format(TStream* str, String path, bool maybezipped2 = false);
	bool recursive_test_stream_format(v8catalog* cat, String path);

	void pagemapfill();
	String pagemaprec_presentation(pagemaprec& pmr);
};
//---------------------------------------------------------------------------
// Известные версии хранилища конфигурации
enum depot_ver
{
	depotVer3 = 3, // 0300000000000000
	depotVer5 = 5, // 0500000000000000
	depotVer6 = 6, // 0600000000000000
	depotVer7 = 7 // 0700000000000000
};

//---------------------------------------------------------------------------
tree* get_treeFromV8file(v8file* f);

//---------------------------------------------------------------------------
#endif

