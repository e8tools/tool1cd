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

#ifndef Class_1CDH
#define Class_1CDH

#include <boost/filesystem.hpp>
#include <vector>

#include "MessageRegistration.h"
#include "db_ver.h"
#include "Parse_tree.h"
#include "V8Object.h"
#include "Field.h"
#include "Index.h"
#include "Table.h"
#include "TableFiles.h"
#include "MemBlock.h"
#include "cfapi/V8File.h"
#include "cfapi/V8Catalog.h"
#include "cfapi/TV8FileStream.h"
#include "SupplierConfig.h"
#include "TableRecord.h"

//---------------------------------------------------------------------------

class T_1CD;
class Table;
class V8Object;
class Field;
struct TableFile;
class TableFiles;
class SupplierConfig;

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

	std::string getver(){
		std::string ss = std::to_string((int32_t)ver1);
		ss += ".";
		ss += std::to_string((int32_t)ver2);
		ss += ".";
		ss += std::to_string((int32_t)ver3);
		ss += ".";
		ss += std::to_string((int32_t)ver4);
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
	pagemaprec(){tab = -1; type = pagetype::lost; number = 0;}
};


// Версии файлов shapshot

enum class snapshot_version {
	Ver1 = 1,
	Ver2 = 2
};

// Известные версии хранилища конфигурации
enum class depot_ver
{
	UnknownVer = 0,
	Ver3 = 3, // 0300000000000000
	Ver5 = 5, // 0500000000000000
	Ver6 = 6, // 0600000000000000
	Ver7 = 7  // 0700000000000000
};

// класс базы 1CD
class T_1CD
{
public:

	typedef std::vector<std::shared_ptr<SupplierConfig>> SupplierConfigs;

	static bool recoveryMode;
	char* locale; // код языка базы
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

	std::string ver;

	T_1CD(const std::string &_filename, MessageRegistrator *mess = nullptr, bool monopoly = true);
	T_1CD();
	~T_1CD();
	bool is_open();
	bool is_infobase() const;
	int32_t get_numtables();
	Table* get_table(int32_t numtable);
	db_ver get_version();

	bool save_config(const boost::filesystem::path &file_name);
	bool save_configsave(const boost::filesystem::path &file_name);
	bool save_depot_config(const std::string &_filename, int32_t ver = 0);
	bool save_part_depot_config(const std::string &_filename, int32_t ver_begin, int32_t ver_end);
	int32_t get_ver_depot_config(int32_t ver); // Получение номера версии конфигурации (0 - последняя, -1 - предпоследняя и т.д.)
	bool save_config_ext(const boost::filesystem::path &file_name, const BinaryGuid &uid, const std::string &hashname);
	bool save_config_ext_db(const boost::filesystem::path &file_name, const std::string &hashname);
		
	bool get_readonly();
	void set_readonly(bool ro);
	void flush();

	bool test_stream_format();
	bool test_list_of_tables(); // проверка списка таблиц (по DBNames)
	void find_lost_objects();
	void find_and_save_lost_objects(boost::filesystem::path &lost_objects);
	bool create_table(const std::string &path); // создание таблицы из файлов импорта таблиц
	bool delete_table(Table* tab);
	bool delete_object(V8Object* ob);
	bool replaceTREF(const std::string &mapfile); // замена значений полей ...TREF во всех таблицах базы
	void find_and_create_lost_tables();
	void restore_DATA_allocation_table(Table* tab);
	bool test_block_by_template(uint32_t testblock, char* tt, uint32_t num, int32_t rlen, int32_t len);
	std::string get_filename() const;
	uint32_t get_pagesize() const;

	SupplierConfigs& supplier_configs();

	bool get_block(void* buf, uint32_t block_number, int32_t blocklen = -1); // буфер принадлежит вызывающей процедуре
	char* get_block(uint32_t block_number) const; // буфер не принадлежит вызывающей стороне (принадлежит memblock)
	char* get_block_for_write(uint32_t block_number, bool read); // буфер не принадлежит вызывающей стороне (принадлежит memblock)
	void set_block_as_free(uint32_t block_number); // пометить блок как свободный
	uint32_t get_free_block(); // получить номер свободного блока (и пометить как занятый)

	const MemBlockManager &getMemBlockManager() const;

private:
	mutable Registrator msreg_m;
	mutable MemBlockManager memBlockManager;
	std::string filename;
	std::shared_ptr<TFileStream> fs;

	db_ver version; // версия базы
	uint32_t pagesize; // размер одной страницы (до версии 8.2.14 всегда 0x1000 (4K), начиная с версии 8.3.8 от 0x1000 (4K) до 0x10000 (64K))
	uint32_t length; // длина базы в блоках
	V8Object* free_blocks; // свободные блоки
	V8Object* root_object; // корневой объект
	int32_t num_tables; // количество таблиц
	Table** tables; // таблицы базы
	bool readonly;
	pagemaprec* pagemap; // Массив длиной length

	SupplierConfigs _supplier_configs; // конфигурации поставщика
	bool supplier_configs_defined {false}; // признак, что был произведен поиск конфигураций поставщика
	void find_supplier_configs();

	bool _is_infobase; // признак информационной базы

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

	void add_supplier_config(TableFile* table_file);

	bool recursive_test_stream_format(Table* t, uint32_t nrec);
	bool recursive_test_stream_format2(Table* t, uint32_t nrec); // для DBSCHEMA
	bool recursive_test_stream_format(TStream *str, const std::string &path, bool maybezipped2 = false);
	bool recursive_test_stream_format(V8Catalog *cat, const std::string &path);

	void pagemapfill();
	std::string pagemaprec_presentation(pagemaprec& pmr);

	depot_ver get_depot_version(const TableRecord &record);

	void assert_i_am_a_repository();
	bool try_save_snapshot(const TableRecord &version_record,
						   int ver,
						   const BinaryGuid &rootobj,
						   const boost::filesystem::path &root_path,
						   const boost::filesystem::path &target_file_path) const;
};

#endif

