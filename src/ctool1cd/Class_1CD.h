//---------------------------------------------------------------------------

#ifndef Class_1CDH
#define Class_1CDH

#include "vector"

#include "MessageRegistration.h"
#include "APIcfBase.h"
#include "db_ver.h"
#include "Parse_tree.h"
#include "V8Object.h"
#include "Field.h"
#include "Index.h"
#include "Table.h"
#include "TableFiles.h"
#include "MemBlock.h"

//---------------------------------------------------------------------------

class T_1CD;
class Table;
class v8object;
class Field;
class table_file;
class TableFiles;

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
enum class pagetype
{
	lost, // потерянная страница (не относится ни к одному объекту)
	root, // корневая страница (страница 0)
	freeroot, // корневая страница таблицы свободных блоков (страница 1)
	freealloc, // страница размещения таблицы свободных блоков
	free, // свободная страница
	rootfileroot, // корневая страница корневого файла (страница 2)
	rootfilealloc, // страница размещения корневого файла
	rootfile, // страница данных корневого файла
	descrroot, // корневая страница файла descr таблицы
	descralloc, // страница размещения файла descr таблицы
	descr, // страница данных файла descr таблицы
	dataroot, // корневая страница файла data таблицы
	dataalloc, // страница размещения файла data таблицы
	data, // страница данных файла data таблицы
	indexroot, // корневая страница файла index таблицы
	indexalloc, // страница размещения файла index таблицы
	index, // страница данных файла index таблицы
	blobroot, // корневая страница файла blob таблицы
	bloballoc, // страница размещения файла blob таблицы
	blob // страница данных файла blob таблицы
};

// Структура принадлежности страницы
struct pagemaprec
{
	int32_t tab; // Индекс в T_1CD::tables, -1 - страница не относится к таблицам
	pagetype type; // тип страницы
	uint32_t number; // номер страницы в своем типе
	pagemaprec(){tab = -1; type = pagetype::lost; number = 0;};
};


// Версии файлов shapshot

enum class snapshot_version {
	Ver1 = 1,
	Ver2 = 2
};

// класс базы 1CD
class T_1CD
{
friend v8object;
friend Table;
friend Index;
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
	Index* get_index(Table* tab, String indexname);

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
enum class depot_ver
{
	Ver3 = 3, // 0300000000000000
	Ver5 = 5, // 0500000000000000
	Ver6 = 6, // 0600000000000000
	Ver7 = 7  // 0700000000000000
};

//---------------------------------------------------------------------------
tree* get_treeFromV8file(v8file* f);

//---------------------------------------------------------------------------
#endif

