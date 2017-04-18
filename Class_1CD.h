//---------------------------------------------------------------------------

#ifndef Class_1CDH
#define Class_1CDH

#include "vector"

#include "MessageRegistration.h"
#include "APIcfBase.h"
//#ifndef getcfname
//#include "ICU.h"
//#endif
#include "db_ver.h"
#include "Parse_tree.h"

//---------------------------------------------------------------------------

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

//String __fastcall GUIDas1C(const unsigned char* fr);
//String __fastcall GUIDasMS(const unsigned char* fr);

class T_1CD;
class table;

class ConfigStorageTableConfig;
class ConfigStorageTableConfigSave;

//unsigned int __fastcall reverse_byte_order(unsigned int value);

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
	unsigned int length;
	int firstblock;
	unsigned int pagesize;

	String __fastcall getver(){
		String ss = (int)ver1;
		ss += ".";
		ss += (int)ver2;
		ss += ".";
		ss += (int)ver3;
		ss += ".";
		ss += (int)ver4;
		return ss;
	}
};

// Структура страницы размещения уровня 1 версий от 8.0 до 8.2.14
struct objtab
{
	int numblocks;
	unsigned int blocks[1023];
};

// Структура страницы размещения уровня 1 версий от 8.3.8
struct objtab838
{
	unsigned int blocks[1]; // реальное количество блоков зависит от размера страницы (pagesize)
};

struct _version_rec
{
	unsigned int version_1; // версия реструктуризации
	unsigned int version_2; // версия изменения
};

struct _version
{
	unsigned int version_1; // версия реструктуризации
	unsigned int version_2; // версия изменения
	unsigned int version_3; // версия изменения 2
};

// структура заголовочной страницы файла данных или файла свободных страниц
struct v8ob
{
	char sig[8]; // сигнатура SIG_OBJ
	unsigned int len; // длина файла
	_version version;
	unsigned int blocks[1018];
};

// структура заголовочной страницы файла данных начиная с версии 8.3.8
struct v838ob_data
{
	unsigned char sig[2]; // сигнатура 0x1C 0xFD (1C File Data?)
	short int fatlevel; // уровень таблицы размещения (0x0000 - в таблице blocks номера страниц с данными, 0x0001 - в таблице blocks номера страниц с таблицами размещения второго уровня, в которых уже, в свою очередь, находятся номера страниц с данными)
	_version version;
	uint64_t len; // длина файла
	unsigned int blocks[1]; // Реальная длина массива зависит от размера страницы и равна pagesize/4-6 (от это 1018 для 4К до 16378 для 64К)
};

// структура заголовочной страницы файла свободных страниц начиная с версии 8.3.8
struct v838ob_free
{
	unsigned char sig[2]; // сигнатура 0x1C 0xFF (1C File Free?)
	short int fatlevel; // 0x0000 пока! но может ... уровень таблицы размещения (0x0000 - в таблице blocks номера страниц с данными, 0x0001 - в таблице blocks номера страниц с таблицами размещения второго уровня, в которых уже, в свою очередь, находятся номера страниц с данными)
	unsigned int version; // ??? предположительно...
	unsigned int blocks[1]; // Реальная длина массива зависит от размера страницы и равна pagesize/4-6 (от это 1018 для 4К до 16378 для 64К)
};


#pragma pack(pop)

// типы внутренних файлов
enum v8objtype
{
	v8ot_unknown = 0, // тип неизвестен
	v8ot_data80 = 1, // файл данных формата 8.0 (до 8.2.14 включительно)
	v8ot_free80 = 2, // файл свободных страниц формата 8.0 (до 8.2.14 включительно)
	v8ot_data838 = 3, // файл данных формата 8.3.8
	v8ot_free838 = 4 // файл свободных страниц формата 8.3.8
};

class v8object
{
friend table;
friend T_1CD;
private:
	MessageRegistrator* err;
	T_1CD* base;

	uint64_t len; // длина объекта. Для типа таблицы свободных страниц - количество свободных блоков
	_version version; // текущая версия объекта
	_version_rec version_rec; // текущая версия записи
	bool new_version_recorded; // признак, что новая версия объекта записана
//	unsigned int version_restr; // версия реструктуризации
//	unsigned int version_edit; // версия изменения
	v8objtype type; // тип и формат файла
	int fatlevel; // Количество промежуточных уровней в таблице размещения
	unsigned int numblocks; // кол-во страниц в корневой таблице размещения объекта
	unsigned int real_numblocks; // реальное кол-во страниц в корневой таблице (только для файлов свободных страниц, может быть больше numblocks)
	unsigned int* blocks; // таблица страниц корневой таблицы размещения объекта (т.е. уровня 0)
	unsigned int block; // номер блока объекта
	char* data; // данные, представляемые объектом, NULL если не прочитаны или len = 0

	static v8object* first;
	static v8object* last;
	v8object* next;
	v8object* prev;
	unsigned int lastdataget; // время (Windows time, в миллисекундах) последнего обращения к данным объекта (data)
	bool lockinmemory;

	void __fastcall set_len(uint64_t _len); // установка новой длины объекта

	void __fastcall init();
	void __fastcall init(T_1CD* _base, int blockNum);

public:
	__fastcall v8object(T_1CD* _base, int blockNum); // конструктор существующего объекта
	__fastcall v8object(T_1CD* _base); // конструктор нового (еще не существующего) объекта
	__fastcall ~v8object();
	char* __fastcall getdata(); // чтение всего объекта целиком, поддерживает кеширование объектов. Буфер принадлежит объекту
	char* __fastcall getdata(void* buf, uint64_t _start, uint64_t _length); // чтение кусочка объекта, поддерживает кеширование блоков. Буфер не принадлежит объекту
	bool __fastcall setdata(void* buf, uint64_t _start, uint64_t _length); // запись кусочка объекта, поддерживает кеширование блоков.
	bool __fastcall setdata(void* buf, uint64_t _length); // запись объекта целиком, поддерживает кеширование блоков.
	bool __fastcall setdata(TStream* stream); // записывает поток целиком в объект, поддерживает кеширование блоков.
	bool __fastcall setdata(TStream* stream, uint64_t _start, uint64_t _length); // запись части потока в объект, поддерживает кеширование блоков.
	uint64_t __fastcall getlen();
	//void __fastcall savetofile();
	void __fastcall savetofile(String filename);
	void __fastcall set_lockinmemory(bool _lock);
	static void __fastcall garbage();
	uint64_t __fastcall get_fileoffset(uint64_t offset); // получить физическое смещение в файле по смещению в объекте
	void __fastcall set_block_as_free(unsigned int block_number); // пометить блок как свободный
	unsigned int __fastcall get_free_block(); // получить номер свободного блока (и пометить как занятый)
	void __fastcall get_version_rec_and_increase(_version* ver); // получает версию очередной записи и увеличивает сохраненную версию объекта
	void __fastcall get_version(_version* ver); // получает сохраненную версию объекта
	void __fastcall write_new_version(); // записывает новую версию объекта
	static v8object* __fastcall get_first();
	static v8object* __fastcall get_last();
	v8object* __fastcall get_next();
	unsigned int __fastcall get_block_number();
	TStream* readBlob(TStream* _str, unsigned int _startblock, unsigned int _length = MAXUINT, bool rewrite = true);
};

#pragma pack(push)
#pragma pack(1)

struct root_80
{
	char lang[8];
	int numblocks;
	unsigned int blocks[1];
};

struct root_81
{
	char lang[32];
	int numblocks;
	unsigned int blocks[1];
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
	tf_version, // RV // 16, 8 версия создания и 8 версия модификации ? каждая версия int(изменения) + int(реструктуризация)
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


class field
{
friend table;
friend T_1CD;
public:
	static bool showGUID;
private:
	MessageRegistrator* err;
	String name;
	type_fields type;
	bool null_exists;
	int length;
	int precision;
	bool case_sensitive;

	table* parent;
	int len; // длина поля в байтах
	int offset; // смещение поля в записи
	static bool showGUIDasMS; // Признак, что GUID надо преобразовывать по стилю MS (иначе по стилю 1С)
	static char buf[];
	static char null_index[];
	static bool null_index_initialized;

public:
	__fastcall field(table* _parent);

	int __fastcall getlen(); // возвращает длину поля в байтах
	String __fastcall getname();
	String __fastcall get_presentation(const char* rec, bool EmptyNull = false, wchar_t Delimiter = 0, bool ignore_showGUID = false, bool detailed = false);
	String __fastcall get_XML_presentation(char* rec, bool ignore_showGUID = false);
	bool __fastcall get_bynary_value(char* buf, bool null, String& value);
	type_fields __fastcall gettype();
	table* __fastcall getparent();
	bool __fastcall getnull_exists();
	int __fastcall getlength();
	int __fastcall getprecision();
	bool __fastcall getcase_sensitive();
	int __fastcall getoffset();
	String __fastcall get_presentation_type();
	bool __fastcall save_blob_to_file(char* rec, String filename, bool unpack);
#ifndef PublicRelease
	unsigned int __fastcall getSortKey(const char* rec, unsigned char* SortKey, int maxlen);
#endif //#ifdef PublicRelease
};

struct index_record
{
	field* field;
	int len;
};

#pragma pack(push)
#pragma pack(1)


// структура одной записи распакованного индекса страницы-листа
struct unpack_index_record
{
	unsigned int _record_number; // номер (индекс) записи в таблице записей
	unsigned char _index[1]; // значение индекса записи. Реальная длина значения определяется полем length класса index
};

// структура заголовка страницы-ветки индексов
struct branch_page_header{
	unsigned short int flags; // offset 0
	unsigned short int number_indexes; // offset 2
	unsigned int prev_page; // offset 4 // для 8.3.8 - это номер страницы (реальное смещение = prev_page * pagesize), до 8.3.8 - это реальное смещение
	unsigned int next_page; // offset 8 // для 8.3.8 - это номер страницы (реальное смещение = next_page * pagesize), до 8.3.8 - это реальное смещение
};

// структура заголовка страницы-листа индексов
struct leaf_page_header{
	short int flags; // offset 0
	unsigned short int number_indexes; // offset 2
	unsigned int prev_page; // offset 4 // для 8.3.8 - это номер страницы (реальное смещение = prev_page * pagesize), до 8.3.8 - это реальное смещение
	unsigned int next_page; // offset 8 // для 8.3.8 - это номер страницы (реальное смещение = next_page * pagesize), до 8.3.8 - это реальное смещение
	unsigned short int freebytes; // offset 12
	unsigned int numrecmask; // offset 14
	unsigned short int leftmask; // offset 18
	unsigned short int rightmask; // offset 20
	unsigned short int numrecbits; // offset 22
	unsigned short int leftbits; // offset 24
	unsigned short int rightbits; // offset 26
	unsigned short int recbytes; // offset 28
};

// Вспомогательная структура для упаковки индексов на странице-листе
struct _pack_index_record
{
	unsigned int numrec;
	unsigned int left;
	unsigned int right;
};

#pragma pack(pop)

// Значения битовых флагов в заголовке стрницы индекса
const short int indexpage_is_root = 1; // Установленный флаг означает, что страница является корневой
const short int indexpage_is_leaf = 2; // Установленный флаг означает, что страница является листом, иначе веткой

class index
{
friend table;
private:
	MessageRegistrator* err;
	table* tbase;
	db_ver version; // версия базы
	unsigned int pagesize; // размер одной стрницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))

	String name;
	bool is_primary;
	int num_records; // количество полей в индексе
	index_record* records;

	uint64_t start; // Смещение в файле индексов блока описания индекса
	uint64_t rootblock; // Смещение в файле индексов корневого блока индекса
	unsigned int length; // длина в байтах одной распакованной записи индекса
	DynamicArray<unsigned int> recordsindex; // динамический массив индексов записей по номеру (только не пустые записи)
	bool recordsindex_complete; // признак заполнености recordsindex
	void __fastcall create_recordsindex();

#ifndef PublicRelease
	void __fastcall dump_recursive(v8object* file_index, TFileStream* f, int level, uint64_t curblock);
	void __fastcall delete_index(const char* rec, const unsigned int phys_numrec); // удаление индекса записи из файла index
	void __fastcall delete_index_record(const char* index_buf, const unsigned int phys_numrec); // удаление одного индекса из файла index
	void __fastcall delete_index_record(const char* index_buf, const unsigned int phys_numrec, uint64_t block, bool& is_last_record, bool& page_is_empty, char* new_last_index_buf, unsigned int& new_last_phys_num); // рекурсивное удаление одного индекса из блока файла index
	void __fastcall write_index(const unsigned int phys_numrecord, const char* rec); // запись индекса записи
	void __fastcall write_index_record(const unsigned int phys_numrecord, const char* index_buf); // запись индекса
	void __fastcall write_index_record(const unsigned int phys_numrecord, const char* index_buf, uint64_t block, int& result, char* new_last_index_buf, unsigned int& new_last_phys_num, char* new_last_index_buf2, unsigned int& new_last_phys_num2, uint64_t& new_last_block2); // рекурсивная запись индекса
#endif //#ifdef PublicRelease

public:
	__fastcall index(table* _base);
	__fastcall ~index();

	String __fastcall getname();
	bool __fastcall get_is_primary();
	int __fastcall get_num_records(); // получить количество полей в индексе
	index_record* __fastcall get_records();

	unsigned int __fastcall get_numrecords(); // получает количество записей, проиндексированных индексом
	unsigned int __fastcall get_numrec(unsigned int num_record); // получает физический индекс записи по порядковому индексу

#ifndef PublicRelease
	void __fastcall dump(String filename);
	void __fastcall calcRecordIndex(const char* rec, char* indexBuf); // вычислить индекс записи rec и поместить в indexBuf. Длина буфера indexBuf должна быть не меньше length
#endif //#ifdef PublicRelease

	unsigned int __fastcall get_rootblock();
	unsigned int __fastcall get_length();

	// распаковывает одну страницу-лист индексов
	// возвращает массив структур unpack_index_record. Количество элементов массива возвращается в number_indexes
	char* __fastcall unpack_leafpage(uint64_t page_offset, unsigned int& number_indexes);

	// распаковывает одну страницу-лист индексов
	// возвращает массив структур unpack_index_record. Количество элементов массива возвращается в number_indexes
	char* __fastcall unpack_leafpage(char* page, unsigned int& number_indexes);

	// упаковывает одну страницу-лист индексов.
	// возвращвет истина, если упаковка произведена, и ложь, если упаковка невозможна.
	bool __fastcall pack_leafpage(char* unpack_index, unsigned int number_indexes, char* page_buf);

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
	table* parent;

	// физический номер записи (для добавленных записей нумерация начинается с phys_numrecords)
	unsigned int numrec;

	// тип изменения записи (изменена, добавлена, удалена)
	changed_rec_type changed_type;

	// следующая измененная запись в списке измененных записей
	changed_rec* next;

	// массив признаков изменения поля (по одному байту на каждое поле, всего num_fields байт)
	char* fields;

	// измененная запись. Для типов полей tf_text (TEXT), tf_string (MEMO) и tf_image (BLOB), если соответствующий признак в fields установлен,
	// содержит указатель на TStream с содержимым поля (или NULL)
	char* rec;

	__fastcall changed_rec(table* _parent, changed_rec_type crt, unsigned int phys_numrecord);
	__fastcall ~changed_rec();
	void __fastcall clear();
};

// структура одного блока в файле file_blob
struct blob_block{
	unsigned int nextblock;
	short int length;
	char data[250];
};

// структура root файла экспорта/импорта таблиц
struct export_import_table_root
{
	bool has_data;
	bool has_blob;
	bool has_index;
	bool has_descr;
	int data_version_1; // версия реструктуризации
	int data_version_2; // версия изменения
	int blob_version_1; // версия реструктуризации
	int blob_version_2; // версия изменения
	int index_version_1; // версия реструктуризации
	int index_version_2; // версия изменения
	int descr_version_1; // версия реструктуризации
	int descr_version_2; // версия изменения
};

class table{
friend field;
friend class index;
friend changed_rec;
friend T_1CD;
private:
	MessageRegistrator* err;
	T_1CD* base;

	v8object* descr_table; // объект с описанием структуры таблицы (только для версий с 8.0 до 8.2.14)
	String description;
	String name;
	int num_fields;
	int num_fields2; // количество элементов в массиве fields
	field** fields;
	int num_indexes;
	class index** indexes;
	bool recordlock;
	v8object* file_data;
	v8object* file_blob;
	v8object* file_index;
	int recordlen; // длина записи (в байтах)
	bool issystem; // Признак системной таблицы (имя таблицы не начинается с подчеркивания)
	int lockinmemory; // счетчик блокировок в памяти

	void __fastcall deletefields();
	void __fastcall deleteindexes();

	changed_rec* ch_rec; // первая измененная запись в списке измененных записей
	unsigned int added_numrecords; // количество добавленных записей в режиме редактирования

	unsigned int phys_numrecords; // физическое количество записей (вместе с удаленными)
	unsigned int log_numrecords; // логическое количество записей (только не удаленные)

	void __fastcall create_file_data(); // создание файла file_data
	void __fastcall create_file_blob(); // создание файла file_blob
	void __fastcall create_file_index(); // создание файла file_index
	void __fastcall refresh_descr_table(); // создание и запись файла описания таблицы
#ifndef PublicRelease

	bool edit; // признак, что таблица находится в режиме редактирования

	void __fastcall delete_data_record(unsigned int phys_numrecord); // удаление записи из файла data
	void __fastcall delete_blob_record(unsigned int blob_numrecord); // удаление записи из файла blob
	void __fastcall delete_index_record(unsigned int phys_numrecord); // удаление всех индексов записи из файла index
	void __fastcall delete_index_record(unsigned int phys_numrecord, char* rec); // удаление всех индексов записи из файла index
	void __fastcall write_data_record(unsigned int phys_numrecord, char* rec); // запись одной записи в файл data
	unsigned int __fastcall write_blob_record(char* blob_record, unsigned int blob_len); // записывает НОВУЮ запись в файл blob, возвращает индекс новой записи
	unsigned int __fastcall write_blob_record(TStream* bstr); //  // записывает НОВУЮ запись в файл blob, возвращает индекс новой записи
	void __fastcall write_index_record(const unsigned int phys_numrecord, const char* rec); // запись индексов записи в файл index
#endif //#ifdef PublicRelease

	bool bad; // признак битой таблицы

public:
	//--> поддержка динамического построения таблицы записей
	//DynamicArray<unsigned int> recordsindex; // динамический массив индексов записей по номеру (только не пустые записи)
	unsigned int* recordsindex; // массив индексов записей по номеру (только не пустые записи)
	bool recordsindex_complete; // признак заполнености recordsindex
	unsigned int numrecords_review; // количество просмотренных записей всего в поиске не пустых
	unsigned int numrecords_found; // количество найденных непустых записей (текущий размер recordsindex)
	//<-- поддержка динамического построения таблицы записей
	void __fastcall fillrecordsindex(); // заполнить recordsindex не динамически


	__fastcall table();
	__fastcall table(T_1CD* _base, int block_descr);
	__fastcall table(T_1CD* _base, String _descr, int block_descr = 0);
	__fastcall ~table();
	void __fastcall init(int block_descr = 0);

	String __fastcall getname();
	String __fastcall getdescription();
	int __fastcall get_numfields();
	int __fastcall get_numindexes();
	field* __fastcall getfield(int numfield);
	class index* __fastcall getindex(int numindex);
	bool __fastcall get_issystem();
	int __fastcall get_recordlen();
	bool __fastcall get_recordlock();

	unsigned int __fastcall get_phys_numrecords(); // возвращает количество записей в таблице всего, вместе с удаленными
	unsigned int __fastcall get_log_numrecords(); // возвращает количество записей в таблице всего, без удаленных
	void __fastcall set_log_numrecords(unsigned int _log_numrecords); //
	unsigned int __fastcall get_added_numrecords();

	char* __fastcall getrecord(unsigned int phys_numrecord, char* buf); // возвращает указатель на запись, буфер принадлежит вызывающей процедуре
	TStream* readBlob(TStream* _str, unsigned int _startblock, unsigned int _length, bool rewrite = true);
	unsigned int readBlob(void* _buf, unsigned int _startblock, unsigned int _length);
	void __fastcall set_lockinmemory(bool _lock);
	//bool __fastcall export_to_xml(String filename, index* curindex, bool blob_to_file, bool unpack);
	bool __fastcall export_to_xml(String filename, bool blob_to_file, bool unpack);

	v8object* __fastcall get_file_data();
	v8object* __fastcall get_file_blob();
	v8object* __fastcall get_file_index();

	__int64 __fastcall get_fileoffset(unsigned int phys_numrecord); // получить физическое смещение в файле записи по номеру

	char* __fastcall get_edit_record(unsigned int phys_numrecord, char* buf); // возвращает указатель на запись, буфер принадлежит вызывающей процедуре
	bool __fastcall get_edit();

	unsigned int __fastcall get_phys_numrec(int ARow, class index* cur_index); // получить физический индекс записи по номеру строки по указанному индексу
	String __fastcall get_file_name_for_field(int num_field, char* rec, unsigned int numrec = 0); // получить имя файла по-умолчанию конкретного поля конкретной записи
	String __fastcall get_file_name_for_record(char* rec); // получить имя файла по-умолчанию конкретной записи
	T_1CD* __fastcall getbase(){return base;};

#ifndef PublicRelease
	void __fastcall begin_edit(); // переводит таблицу в режим редактирования
	void __fastcall cancel_edit(); // переводит таблицу в режим просмотра и отменяет все изменения
	void __fastcall end_edit(); // переводит таблицу в режим просмотра и сохраняет все изменения
	changed_rec_type __fastcall get_rec_type(unsigned int phys_numrecord);
	changed_rec_type __fastcall get_rec_type(unsigned int phys_numrecord, int numfield);
	void __fastcall set_edit_value(unsigned int phys_numrecord, int numfield, bool null, String value, TStream* st = NULL);
	void __fastcall restore_edit_value(unsigned int phys_numrecord, int numfield);
	void __fastcall set_rec_type(unsigned int phys_numrecord, changed_rec_type crt);

	void __fastcall export_table(String path);
	void __fastcall import_table(String path);
	void __fastcall import_table2(String path);

	void __fastcall delete_record(unsigned int phys_numrecord); // удаление записи
	void __fastcall insert_record(char* rec); // добавление записи
	void __fastcall update_record(unsigned int phys_numrecord, char* rec, char* changed_fields); // изменение записи
	char* __fastcall get_record_template_test();
#endif //#ifdef PublicRelease

};

//---------------------------------------------------------------------------
// Структура адреса файла таблицы-контейнера файлов
struct table_blob_file
{
	unsigned int blob_start;
	unsigned int blob_length;
};

//---------------------------------------------------------------------------
// Структура записи таблицы контейнера файлов
struct table_rec
{
	String name;
	table_blob_file addr;
	int partno;
	FILETIME ft_create;
	FILETIME ft_modify;
};

//---------------------------------------------------------------------------
// Структура файла таблицы контейнера файлов
struct table_file
{
	table* t;
	String name; // Имя, как оно хранится в таблице
	unsigned int maxpartno;
	table_blob_file* addr;
	FILETIME ft_create;
	FILETIME ft_modify;

	__fastcall table_file(table* _t, const String& _name, unsigned int _maxpartno);
	__fastcall ~table_file();
};

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFiles
{
private:
	table* tab;
	std::map<String,table_file*> allfiles;
	char* rec;
	bool ready;

	bool __fastcall test_table();
public:
	__fastcall TableFiles(table* t);
	virtual __fastcall ~TableFiles();
	bool __fastcall getready(){return ready;};
	table_file* __fastcall getfile(const String& name);
	table* __fastcall gettable(){return tab;};

	// __property std::map<String,table_file*> files = {read = allfiles};
};

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFileStream : public TStream
{
private:
	__int64 curoffset;
	table_file* tablefile;
	TStream** streams;
public:
	__fastcall TableFileStream(table_file* tf);
	virtual __fastcall ~TableFileStream();

	virtual int __fastcall Read(void *Buffer, int Count);
	virtual int __fastcall Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count);
	virtual int __fastcall Write(const void *Buffer, int Count){throw(Exception(L"Write read-only stream"));};
	virtual int __fastcall Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){throw(Exception(L"Write read-only stream"));};
	virtual int __fastcall Seek(int Offset, System::Word Origin);
	virtual __int64 __fastcall Seek(const __int64 Offset, TSeekOrigin Origin);
};


// класс кешированного блока в памяти
// первый в цепочке кешированный блок - тот, к которому наиболее давно обращались
// последний в цепочке - с самым последним обращением
class memblock
{
friend T_1CD;
private:
	char* buf; // указатель на блок в памяти
	static unsigned int pagesize; // размер одной стрницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	unsigned int numblock;
	memblock* next;
	memblock* prev;
	TFileStream* file; // файл, которому принадлежит блок
	bool is_changed; // признак, что блок изменен (требует записи)

	static memblock* first;
	static memblock* last;
	static unsigned int maxcount; // максимальное количество кешированных блоков
	static unsigned int numblocks; // количество значащих элементов в массиве memblocks (равно количеству блоков в файле *.1CD)
	static unsigned int array_numblocks; // количество элементов в массиве memblocks (больше или равно количеству блоков в файле *.1CD)
	static unsigned int delta; // шаг увеличения массива memblocks
	static memblock** memblocks; // указатель на массив указателей memblock (количество равно количеству блоков в файле *.1CD)

	unsigned int lastdataget; // время (Windows time, в миллисекундах) последнего обращения к данным объекта (data)
	char* __fastcall getblock(bool for_write); // получить блок для чтения или для записи
	__fastcall memblock(TFileStream* fs, unsigned int _numblock, bool for_write, bool read);
	__fastcall ~memblock();
	static void __fastcall add_block();
	void __fastcall write();

public:
	static unsigned int count; // текущее количество кешированных блоков

	static void __fastcall garbage();
	static char* __fastcall getblock(TFileStream* fs, unsigned int _numblock);
	static char* __fastcall getblock_for_write(TFileStream* fs, unsigned int _numblock, bool read);
	static void __fastcall create_memblocks(unsigned int _numblocks);
	static void __fastcall delete_memblocks();
	static unsigned int __fastcall get_numblocks();
	static void __fastcall flush();
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
	__int64 offset; // смещение файла в файле *.pck
};
#pragma pack(pop)

// Структура для одной пары файлов *.ind и *.pck в каталоге data\pack\ хранилища 8.3
struct _packdata
{
	TFileStream* pack; // открытый на чтение файл *.pck
	unsigned int count; // кол-во записей (объектов) в файле *.pck
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
	int tab; // Индекс в T_1CD::tables, -1 - страница не относится к таблицам
	pagetype type; // тип страницы
	unsigned int number; // номер страницы в своем типе
	pagemaprec(){tab = -1; type = pt_lost; number = 0;};
};


// класс базы 1CD
class T_1CD
{
friend v8object;
friend table;
friend class index;
friend field;
public:
	static bool recoveryMode;
private:
	MessageRegistrator* err;
	String filename;
	TFileStream* fs;

	db_ver version; // версия базы
	unsigned int pagesize; // размер одной стрницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	unsigned int length; // длина базы в блоках
	v8object* free_blocks; // свободные блоки
	v8object* root_object; // корневой объект
	int num_tables; // количество таблиц
	table** tables; // таблицы базы
	bool readonly;
//#ifndef PublicRelease
//	ICU* icu;
//#endif //#ifdef PublicRelease
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

	void __fastcall init();
	bool __fastcall getblock(void* buf, unsigned int block_number, int blocklen = -1); // буфер принадлежит вызывающей процедуре
	char*  __fastcall getblock(unsigned int block_number); // буфер не принадлежит вызывающей стороне (принадлежит memblock)
	char*  __fastcall getblock_for_write(unsigned int block_number, bool read); // буфер не принадлежит вызывающей стороне (принадлежит memblock)
	void __fastcall set_block_as_free(unsigned int block_number); // пометить блок как свободный
	unsigned int __fastcall get_free_block(); // получить номер свободного блока (и пометить как занятый)

	void __fastcall add_supplier_config(table_file* file);

	bool __fastcall recursive_test_stream_format(table* t, unsigned int nrec);
	bool __fastcall recursive_test_stream_format2(table* t, unsigned int nrec); // для DBSCHEMA
	bool __fastcall recursive_test_stream_format(TStream* str, String path, bool maybezipped2 = false);
	bool __fastcall recursive_test_stream_format(v8catalog* cat, String path);

	void __fastcall pagemapfill();
	String __fastcall pagemaprec_presentation(pagemaprec& pmr);
public:
	char* locale; // код языка базы
	bool is_infobase; // признак информационной базы
	bool is_depot; // признак хранилища конфигурации

	// Таблицы информационной базы
	table* table_config;
	table* table_configsave;
	table* table_params;
	table* table_files;
	table* table_dbschema;
	table* table_configcas;
	table* table_configcassave;
	table* table__extensionsinfo;

//	__property TableFiles* files_config = {read = get_files_config};
//	__property TableFiles* files_configsave = {read = get_files_configsave};
//	__property TableFiles* files_params = {read = get_files_params};
//	__property TableFiles* files_files = {read = get_files_files};
//	__property TableFiles* files_configcas = {read = get_files_configcas};
//	__property TableFiles* files_configcassave = {read = get_files_configcassave};

	// таблицы - хранилища файлов
	ConfigStorageTableConfig* cs_config;
	ConfigStorageTableConfigSave* cs_configsave;

	// Таблицы хранилища конфигураций
	table* table_depot;
	table* table_users;
	table* table_objects;
	table* table_versions;
	table* table_labels;
	table* table_history;
	table* table_lastestversions;
	table* table_externals;
	table* table_selfrefs;
	table* table_outrefs;

	String ver;

	std::vector<SupplierConfig> supplier_configs; // конфигурации поставщика
	bool supplier_configs_defined; // признак, что был произведен поиск конфигураций поставщика

	__fastcall T_1CD(String _filename, MessageRegistrator* _err = NULL, bool monopoly = true);
	__fastcall T_1CD();
	__fastcall ~T_1CD();
	bool __fastcall is_open();
	int __fastcall get_numtables();
	table* __fastcall gettable(int numtable);
	db_ver __fastcall getversion();

	bool __fastcall save_config(String filename);
	bool __fastcall save_configsave(String filename);
	void __fastcall find_supplier_configs();
	bool __fastcall save_supplier_configs(unsigned int numcon, const String& filename);
	bool __fastcall save_depot_config(const String& _filename, int ver = 0);
	bool __fastcall save_part_depot_config(const String& _filename, int ver_begin, int ver_end);
	int __fastcall get_ver_depot_config(int ver); // Получение номера версии конфигурации (0 - последняя, -1 - предпоследняя и т.д.)
	bool __fastcall save_config_ext(const String& _filename, const TGUID& uid, const String& hashname);
	bool __fastcall save_config_ext_db(const String& _filename, const String& hashname);

	field* get_field(table* tab, String fieldname);
	class index* get_index(table* tab, String indexname);

	bool __fastcall get_readonly();
	void __fastcall set_readonly(bool ro);
	void __fastcall flush();

	bool __fastcall test_stream_format();
	bool __fastcall test_list_of_tables(); // проверка списка таблиц (по DBNames)
#ifndef PublicRelease
	void __fastcall find_lost_objects();
	void __fastcall find_and_save_lost_objects();
	bool __fastcall create_table(String path); // создание таблицы из файлов импорта таблиц
	bool __fastcall delete_table(table* tab);
	bool __fastcall delete_object(v8object* ob);
	bool __fastcall replaceTREF(String mapfile); // замена значений полей ...TREF во всех таблицах базы
	void __fastcall find_and_create_lost_tables();
	void __fastcall restore_DATA_allocation_table(table* tab);
	bool __fastcall test_block_by_template(unsigned int testblock, char* tt, unsigned int num, int rlen, int len);
#endif //#ifdef PublicRelease
	String& __fastcall getfilename(){return filename;};
	unsigned int getpagesize(){return pagesize;};

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
tree* __fastcall get_treeFromV8file(v8file* f);

//---------------------------------------------------------------------------
#endif

