//---------------------------------------------------------------------------

#include <vector>
#include <System.IOUtils.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <memory>

#include "UZLib.h"
#include "Class_1CD.h"
#include "Base64.h"
#include "Common.h"
#include "TempStream.h"
#include "ConfigStorage.h"
#include "Constants.h"
#include "CRC32.h"
#include "PackDirectory.h"
#include "TableIterator.h"

using namespace std;
using namespace System;

extern Registrator msreg_g;

bool T_1CD::recoveryMode = false;

const uint32_t ONE_GB = 1073741824;

//********************************************************
// Функции


//********************************************************
// Класс T_1CD

//---------------------------------------------------------------------------
bool T_1CD::getblock(void* buf, uint32_t block_number, int32_t blocklen)
{
	if(!fs) return false;
	if(blocklen < 0) blocklen = pagesize;
	if(block_number >= length)
	{
		throw DetailedException("Попытка чтения блока за пределами файла.")
			.add_detail("Индекс блока", to_hex_string(block_number))
			.add_detail("Всего блоков", to_hex_string(length));
	}

	memcpy(buf, MemBlock::getblock(fs, block_number), blocklen);
	return true;
}

//---------------------------------------------------------------------------
char*  T_1CD::getblock(uint32_t block_number)
{
	if(!fs) return nullptr;
	if(block_number >= length)
	{
		throw DetailedException("Попытка чтения блока за пределами файла.")
			.add_detail("Индекс блока", to_hex_string(block_number))
			.add_detail("Всего блоков", to_hex_string(length));
		return nullptr;
	}

	return MemBlock::getblock(fs, block_number);
}

//---------------------------------------------------------------------------
char*  T_1CD::getblock_for_write(uint32_t block_number, bool read)
{
	v8con* bc;


	if(!fs) return nullptr;
	if(block_number > length)
	{
		throw DetailedException("Попытка получения блока за пределами файла базы.")
			.add_detail("Индекс блока", to_hex_string(block_number))
			.add_detail("Всего блоков", to_hex_string(length));
		return nullptr;
	}

	if(block_number == length)
	{
		length++;
		fs->SetSize(fs->GetSize() + pagesize);
		bc = (v8con*)getblock_for_write(0, true);
		bc->length = length;
	}

	return MemBlock::getblock_for_write(fs, block_number, read);
}

//---------------------------------------------------------------------------
int32_t T_1CD::get_numtables()
{
	return num_tables;
}

//---------------------------------------------------------------------------
Table* T_1CD::gettable(int32_t numtable)
{
	if(numtable >= num_tables)
	{
		throw DetailedException("Попытка получения таблицы по номеру, превышающему количество таблиц")
			.add_detail("Количество таблиц", num_tables)
			.add_detail("Номер таблицы", numtable + 1);
		return nullptr;
	}
	return tables[numtable];
}

//---------------------------------------------------------------------------
void T_1CD::init()
{
	filename    = "";
	fs          = nullptr;
	free_blocks = nullptr;
	root_object = nullptr;
	tables      = nullptr;
	num_tables  = 0;
	table_config     = nullptr;
	table_configsave = nullptr;
	table_params     = nullptr;
	table_files      = nullptr;
	table_dbschema   = nullptr;
	table_configcas  = nullptr;
	table_configcassave   = nullptr;
	table__extensionsinfo = nullptr;

	_files_config     = nullptr;
	_files_configsave = nullptr;
	_files_params     = nullptr;
	_files_files      = nullptr;
	_files_configcas  = nullptr;
	_files_configcassave = nullptr;

	cs_config     = nullptr;
	cs_configsave = nullptr;

	table_depot    = nullptr;
	table_users    = nullptr;
	table_objects  = nullptr;
	table_versions = nullptr;
	table_labels   = nullptr;
	table_history  = nullptr;
	table_lastestversions = nullptr;
	table_externals = nullptr;
	table_selfrefs  = nullptr;
	table_outrefs   = nullptr;

	supplier_configs_defined = false;
	locale                   = nullptr;

	_is_infobase = false;
	is_depot    = false;

	pagemap  = nullptr;
	version  = db_ver::ver8_2_14_0;
	pagesize = DEFAULT_PAGE_SIZE;
	length   = 0;
	readonly = true;
}

//---------------------------------------------------------------------------
T_1CD::T_1CD()
{
	init();
}

//---------------------------------------------------------------------------
T_1CD::~T_1CD()
{
	if(free_blocks)
	{
		delete free_blocks;
		free_blocks = nullptr;
	}

	if(root_object)
	{
		delete root_object;
		root_object = nullptr;
	}

	delete cs_config;
	delete cs_configsave;

	delete _files_config;
	delete _files_configsave;
	delete _files_params;
	delete _files_files;
	delete _files_configcas;
	delete _files_configcassave;

	if(tables)
	{
		for(int32_t i = 0; i < num_tables; i++) delete tables[i];
		delete[] tables;
		tables = nullptr;
	}
	num_tables = 0;

	// сначала закрываем кэшированные блоки (измененные блоки записывают себя в файл) ...
	MemBlock::delete_memblocks();

	// ... и только затем закрываем файл базы.
	if(fs)
	{
		delete fs;
		fs = nullptr;
	}

	delete[] locale;
	if(pagemap) delete[] pagemap;
}

//---------------------------------------------------------------------------
T_1CD::T_1CD(String _filename, MessageRegistrator* mess, bool _monopoly)
{
	char* b = nullptr;
	uint32_t* table_blocks = nullptr;
	int32_t i, j;
	TMemoryStream* tstr = nullptr;
	root_80* root80 = nullptr;
	root_81* root81 = nullptr;

	msreg_m.AddMessageRegistrator(mess);

	init();

	filename = System::Ioutils::TPath::GetFullPath(_filename);

	try
	{
		if(_monopoly) fs = new TFileStream(boost::filesystem::path(filename), fmOpenReadWrite | fmShareDenyWrite);
		else fs = new TFileStream(boost::filesystem::path(filename), fmOpenRead | fmShareDenyNone);
	}
	catch(...)
	{
		throw DetailedException("Ошибка открытия файла базы (файл открыт другой программой?)");
		fs = nullptr;
		return;
	}

	v8con* cont = new v8con;
	fs->Read(cont, sizeof(v8con));

	if(memcmp(&(cont->sig), SIG_CON, 8) != 0)
	{
		throw DetailedException("Файл не является базой 1С (сигнатура не равна \"1CDBMSV8\")");
		delete fs;
		fs = nullptr;
		delete cont;
		return;
	}

	pagesize = DEFAULT_PAGE_SIZE;
	ver = cont->getver();
#ifndef delic
	if(ver == "8.0.3.0"){
		version = db_ver::ver8_0_3_0;
		readonly = true;
	}
	else if(ver == "8.0.5.0"){
		version = db_ver::ver8_0_5_0;
		readonly = true;
	}
	else if(ver == "8.1.0.0")
	{
		version = db_ver::ver8_1_0_0;
		readonly = !_monopoly;
	}
	else if(ver == "8.2.0.0")
	{
		version = db_ver::ver8_2_0_0;
		readonly = !_monopoly;
	}
	else
#endif
		 if(ver == "8.2.14.0")
	{
		version = db_ver::ver8_2_14_0;
		readonly = !_monopoly;
	}
	else if(ver == "8.3.8.0")
	{
		version = db_ver::ver8_3_8_0;
		readonly = !_monopoly;
		pagesize = cont->pagesize;
	}
	else
	{
		throw DetailedException("Неподдерживаемая версия базы 1С").add_detail("Версия базы", ver);
		delete fs;
		fs = nullptr;
		delete cont;
		return;
	}

	length = fs->GetSize() / pagesize;
	if((int64_t)length * pagesize != fs->GetSize())
	{
		throw DetailedException(String("Длина файла базы не кратна длине страницы (") + to_hex_string(pagesize) + ")")
				.add_detail("Длина файла", to_hex_string(fs->GetSize()));
		delete fs;
		fs = nullptr;
		return;
	}

	MemBlock::pagesize = pagesize;
	MemBlock::maxcount = ONE_GB / pagesize; // гигабайт
	MemBlock::create_memblocks(length);

	if(length != cont->length)
	{
		throw DetailedException("Длина файла в блоках и количество блоков в заголовке не равны")
				.add_detail("Длина файла в блоках", length)
				.add_detail("Блоков в заголовке", cont->length);
	}

	free_blocks = new v8object(this, 1);
	root_object = new v8object(this, 2);


	if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0)
	{
		root80 = (root_80*)root_object->getdata();

		locale = new char[strlen(root80->lang) + 1];

#if defined (_MSC_VER)
		strcpy_s(locale, strlen(root80->lang) + 1, root80->lang);
#else
		strcpy(locale, root80->lang);
#endif

		num_tables = root80->numblocks;
		table_blocks = &(root80->blocks[0]);
	}
	else
	{
		if(version >= db_ver::ver8_3_8_0)
		{
			tstr = new TMemoryStream;
			root_object->readBlob(tstr, 1);
			b = new char[tstr->GetSize()];
			memcpy(b, tstr->GetMemory(), tstr->GetSize());
			root81 = (root_81*)b;
		}
		else
		{
			root81 = (root_81*)root_object->getdata();
		}

		locale = new char[strlen(root81->lang) + 1];
#if defined (_MSC_VER)
		strcpy_s(locale, strlen(root81->lang) + 1, root81->lang);
#else
		strcpy(locale, root81->lang);
#endif

		num_tables = root81->numblocks;
		table_blocks = &(root81->blocks[0]);

	}

	tables = new Table*[num_tables];
	for(i = 0, j = 0; i < num_tables; i++)
	{
		if(version < db_ver::ver8_3_8_0)
		{
			tables[j] = new Table(this, table_blocks[i]);
		}
		else
		{
			root_object->readBlob(tstr, table_blocks[i]);
			tables[j] = new Table(this, String((char*)(tstr->GetMemory()), tstr->GetSize()), table_blocks[i]);
		}
		if(tables[j]->bad)
		{
			delete tables[j];
			continue;
		}
		if(!CompareIC(tables[j]->getname(), "CONFIG")) table_config = tables[j];
		if(!CompareIC(tables[j]->getname(), "CONFIGSAVE")) table_configsave = tables[j];
		if(!CompareIC(tables[j]->getname(), "PARAMS")) table_params = tables[j];
		if(!CompareIC(tables[j]->getname(), "FILES")) table_files = tables[j];
		if(!CompareIC(tables[j]->getname(), "DBSCHEMA")) table_dbschema = tables[j];
		if(!CompareIC(tables[j]->getname(), "CONFIGCAS")) table_configcas = tables[j];
		if(!CompareIC(tables[j]->getname(), "CONFIGCASSAVE")) table_configcassave = tables[j];
		if(!CompareIC(tables[j]->getname(), "_EXTENSIONSINFO")) table__extensionsinfo = tables[j];

		if(!CompareIC(tables[j]->getname(), "DEPOT")) table_depot = tables[j];
		if(!CompareIC(tables[j]->getname(), "USERS")) table_users = tables[j];
		if(!CompareIC(tables[j]->getname(), "OBJECTS")) table_objects = tables[j];
		if(!CompareIC(tables[j]->getname(), "VERSIONS")) table_versions = tables[j];
		if(!CompareIC(tables[j]->getname(), "LABELS")) table_labels = tables[j];
		if(!CompareIC(tables[j]->getname(), "HISTORY")) table_history = tables[j];
		if(!CompareIC(tables[j]->getname(), "LASTESTVERSIONS")) table_lastestversions = tables[j];
		if(!CompareIC(tables[j]->getname(), "EXTERNALS")) table_externals = tables[j];
		if(!CompareIC(tables[j]->getname(), "SELFREFS")) table_selfrefs = tables[j];
		if(!CompareIC(tables[j]->getname(), "OUTREFS")) table_outrefs = tables[j];

		if(j % 10 == 0) msreg_m.Status(String("Чтение таблиц ") + j);
		j++;
	}
	msreg_m.Status(String("Чтение таблиц ") + j);
	num_tables = j;

	if(version >= db_ver::ver8_3_8_0)
	{
		delete[] b;
		delete tstr;
	}

#ifdef getcfname
	if(!table_config) throw DetailedException("Отсутствует таблица CONFIG");
#else
#ifdef delic
	if(!table_params) throw DetailedException("Отсутствует таблица PARAMS");
#ifdef delicfiles
	if(!table_config) throw DetailedException("Отсутствует таблица FILES");
	if(!table_config) throw DetailedException("Отсутствует таблица CONFIG");
#endif
#else
	if(!table_config && !table_configsave && !table_params && !table_files && !table_dbschema)
	{
		if(!table_depot && !table_users && !table_objects && !table_versions && !table_labels && !table_history && !table_lastestversions && !table_externals && !table_selfrefs && !table_outrefs)
		{
			msreg_m.AddMessage("База не является информационной базой 1С", MessageState::Info);
		}
		else
		{
			is_depot = true;
			if(!table_depot) throw DetailedException("Отсутствует таблица DEPOT");
			if(!table_users) throw DetailedException("Отсутствует таблица USERS");
			if(!table_objects) throw DetailedException("Отсутствует таблица OBJECTS");
			if(!table_versions) throw DetailedException("Отсутствует таблица VERSIONS");
			if(!table_labels) throw DetailedException("Отсутствует таблица LABELS");
			if(!table_history) throw DetailedException("Отсутствует таблица HISTORY");
			if(!table_lastestversions) throw DetailedException("Отсутствует таблица LASTESTVERSIONS");
			if(!table_externals) throw DetailedException("Отсутствует таблица EXTERNALS");
			if(!table_selfrefs) throw DetailedException("Отсутствует таблица SELFREFS");
			if(!table_outrefs) throw DetailedException("Отсутствует таблица OUTREFS");
			FieldType::showGUIDasMS = true; // TODO: wat??
		}
	}
	else
	{
		_is_infobase = true;
		if(!table_config) throw DetailedException("Отсутствует таблица CONFIG");
		if(!table_configsave) throw DetailedException("Отсутствует таблица CONFIGSAVE");
		if(!table_params) throw DetailedException("Отсутствует таблица PARAMS");
		if(!table_files) throw DetailedException("Отсутствует таблица FILES");
		if(!table_dbschema) throw DetailedException("Отсутствует таблица DBSCHEMA");
	}
#endif //#ifdef delic
#endif //#ifdef getcfname

	delete cont;
}

//---------------------------------------------------------------------------
bool T_1CD::is_open()
{
	return fs != nullptr;
}

//---------------------------------------------------------------------------
db_ver T_1CD::getversion()
{
	return version;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config(const boost::filesystem::path& file_name)
{
	if(!cs_config) {
		cs_config = new ConfigStorageTableConfig(get_files_config());
	}

	if(!cs_config->getready()) {
		return false;
	}

	return cs_config->save_config(file_name);
}

//---------------------------------------------------------------------------
bool T_1CD::save_configsave(const boost::filesystem::path& file_name)
{
	if(!cs_configsave) {
		cs_configsave = new ConfigStorageTableConfigSave(get_files_config(), get_files_configsave());
	}

	if(!cs_configsave->getready()) {
		return false;
	}

	return cs_configsave->save_config(file_name);
}

//---------------------------------------------------------------------------
void T_1CD::find_supplier_configs()
{
	// Состоит из 36(GUID) + 1(.) + 36(GUID)
	constexpr int32_t SUPPLIER_CONFIG_NAME_LEN = 73;

	for(auto& config_save: get_files_configsave()->files()) {
		if(config_save.first.size() == SUPPLIER_CONFIG_NAME_LEN) {
			 add_supplier_config(config_save.second);
		}
	}

	for(auto& config : get_files_config()->files()) {
		if(config.first.size() == SUPPLIER_CONFIG_NAME_LEN) {
			add_supplier_config(config.second);
		}
	}

	supplier_configs_defined = true;
}

//---------------------------------------------------------------------------
void T_1CD::add_supplier_config(TableFile* table_file)
{
	auto sup_conf = SupplierConfig::create_supplier_config(table_file);
	_supplier_configs.push_back(sup_conf);
}

//---------------------------------------------------------------------------
bool T_1CD::get_readonly()
{
	return readonly;
}

//---------------------------------------------------------------------------
void T_1CD::set_block_as_free(uint32_t block_number)
{
	free_blocks->set_block_as_free(block_number);
}

//---------------------------------------------------------------------------
uint32_t T_1CD::get_free_block()
{
	return free_blocks->get_free_block();
}

//---------------------------------------------------------------------------
void T_1CD::flush()
{
	MemBlock::flush();
}

//---------------------------------------------------------------------------

void T_1CD::find_lost_objects()
{
	uint32_t i;
	char buf[8];
	v8object* v8obj;
	bool block_is_find;

	for(i = 1; i < length; i++)
	{
		getblock(buf, i, 8);
		if(memcmp(buf, SIG_OBJ, 8) == 0)
		{
			block_is_find = false;
			for(v8obj = v8object::get_first(); v8obj; v8obj = v8obj->get_next())
			{
				if(v8obj->get_block_number() == i)
				{
					block_is_find = true;
					break;
				}
			}
			if (!block_is_find) {
				msreg_m.AddMessage("Найден потерянный объект", MessageState::Info)
						.with("Номер блока", to_hex_string(i));
			}
		}
	}
	msreg_m.AddMessage("Поиск потерянных объектов завершен", MessageState::Succesfull);
}

//---------------------------------------------------------------------------
bool T_1CD::test_stream_format()
{
	uint32_t i;
	bool result;
	bool res;

	// CONFIGSAVE
	if(!table_config)
	{
		throw DetailedException("Ошибка тестирования. В базе нет таблицы CONFIG");
	}

	if(table_config->get_numfields() < 6)
	{
		throw DetailedException("Ошибка тестирования. В таблице CONFIG меньше 6 полей")
			.add_detail("Кол-во полей", table_config->get_numfields());
	}

	if(table_config->get_numfields() > 7)
	{
		throw DetailedException("Ошибка тестирования. В таблице CONFIG больше 7 полей")
			.add_detail("Кол-во полей", table_config->get_numfields());
	}

	if (CompareIC(table_config->getfield(0)->getname(), "FILENAME"))
	{
		throw DetailedException("Ошибка тестирования. Первое поле таблицы CONFIG не FILENAME")
			.add_detail("Поле", table_config->getfield(0)->getname());
	}

	if (CompareIC(table_config->getfield(1)->getname(), "CREATION"))
	{
		throw DetailedException("Ошибка тестирования. Второе поле таблицы CONFIG не CREATION")
			.add_detail("Поле", table_config->getfield(1)->getname());
	}

	if (CompareIC(table_config->getfield(2)->getname(), "MODIFIED"))
	{
		throw DetailedException("Ошибка тестирования. Третье поле таблицы CONFIG не MODIFIED")
			.add_detail("Поле", table_config->getfield(2)->getname());
	}

	if (CompareIC(table_config->getfield(3)->getname(), "ATTRIBUTES"))
	{
		throw DetailedException("Ошибка тестирования. Четвертое поле таблицы CONFIG не ATTRIBUTES")
			.add_detail("Поле", table_config->getfield(3)->getname());
	}

	if (CompareIC(table_config->getfield(4)->getname(), "DATASIZE"))
	{
		throw DetailedException("Ошибка тестирования. Пятое поле таблицы CONFIG не DATASIZE")
			.add_detail("Поле", table_config->getfield(4)->getname());
	}

	if (CompareIC(table_config->getfield(5)->getname(), "BINARYDATA"))
	{
		throw DetailedException("Ошибка тестирования. Шестое поле таблицы CONFIG не BINARYDATA")
			.add_detail("Поле", table_config->getfield(5)->getname());
	}

	if(table_config->get_numfields() > 6)
	{
		if (CompareIC(table_config->getfield(6)->getname(), "PARTNO"))
		{
			throw DetailedException("Ошибка тестирования. Седьмое поле таблицы CONFIG не PARTNO")
				.add_detail("Поле", table_config->getfield(6)->getname());
		}
	}

	// CONFIGSAVE
	if(!table_configsave)
	{
		throw DetailedException("Ошибка тестирования. В базе нет таблицы CONFIGSAVE");
	}

	if(table_configsave->get_numfields() < 6)
	{
		throw DetailedException("Ошибка тестирования. В таблице CONFIGSAVE меньше 6 полей")
			.add_detail("Кол-во полей", table_configsave->get_numfields());
	}

	if(table_configsave->get_numfields() > 7)
	{
		throw DetailedException("Ошибка тестирования. В таблице CONFIGSAVE больше 7 полей")
			.add_detail("Кол-во полей", table_configsave->get_numfields());
	}

	if (CompareIC(table_configsave->getfield(0)->getname(), "FILENAME"))
	{
		throw DetailedException("Ошибка тестирования. Первое поле таблицы CONFIGSAVE не FILENAME")
			.add_detail("Поле", table_configsave->getfield(0)->getname());
	}

	if (CompareIC(table_configsave->getfield(1)->getname(), "CREATION"))
	{
		throw DetailedException("Ошибка тестирования. Второе поле таблицы CONFIGSAVE не CREATION")
			.add_detail("Поле", table_configsave->getfield(1)->getname());
	}

	if (CompareIC(table_configsave->getfield(2)->getname(), "MODIFIED"))
	{
		throw DetailedException("Ошибка тестирования. Третье поле таблицы CONFIGSAVE не MODIFIED")
			.add_detail("Поле", table_configsave->getfield(2)->getname());
	}

	if (CompareIC(table_configsave->getfield(3)->getname(), "ATTRIBUTES"))
	{
		throw DetailedException("Ошибка тестирования. Четвертое поле таблицы CONFIGSAVE не ATTRIBUTES")
			.add_detail("Поле", table_configsave->getfield(3)->getname());
	}

	if (CompareIC(table_configsave->getfield(4)->getname(), "DATASIZE"))
	{
		throw DetailedException("Ошибка тестирования. Пятое поле таблицы CONFIGSAVE не DATASIZE")
			.add_detail("Поле", table_configsave->getfield(4)->getname());
	}

	if (CompareIC(table_configsave->getfield(5)->getname(), "BINARYDATA"))
	{
		throw DetailedException("Ошибка тестирования. Шестое поле таблицы CONFIGSAVE не BINARYDATA")
			.add_detail("Поле", table_configsave->getfield(5)->getname());
	}

	if(table_configsave->get_numfields() > 6)
	{
		if (CompareIC(table_configsave->getfield(6)->getname(), "PARTNO"))
		{
			throw DetailedException("Ошибка тестирования. Седьмое поле таблицы CONFIGSAVE не PARTNO")
				.add_detail("Поле", table_configsave->getfield(6)->getname());
		}
	}

	// PARAMS
	if(!table_params)
	{
		throw DetailedException("Ошибка тестирования. В базе нет таблицы PARAMS");
	}

	if(table_params->get_numfields() < 6)
	{
		throw DetailedException("Ошибка тестирования. В таблице PARAMS меньше 6 полей")
			.add_detail("Кол-во полей", table_params->get_numfields());
	}

	if(table_params->get_numfields() > 7)
	{
		throw DetailedException("Ошибка тестирования. В таблице PARAMS больше 7 полей")
			.add_detail("Кол-во полей", table_params->get_numfields());
	}

	if (CompareIC(table_params->getfield(0)->getname(), "FILENAME"))
	{
		throw DetailedException("Ошибка тестирования. Первое поле таблицы PARAMS не FILENAME")
			.add_detail("Поле", table_params->getfield(0)->getname());
	}

	if (CompareIC(table_params->getfield(1)->getname(), "CREATION"))
	{
		throw DetailedException("Ошибка тестирования. Второе поле таблицы PARAMS не CREATION")
			.add_detail("Поле", table_params->getfield(1)->getname());
	}

	if (CompareIC(table_params->getfield(2)->getname(), "MODIFIED"))
	{
		throw DetailedException("Ошибка тестирования. Третье поле таблицы PARAMS не MODIFIED")
			.add_detail("Поле", table_params->getfield(2)->getname());
	}

	if (CompareIC(table_params->getfield(3)->getname(), "ATTRIBUTES"))
	{
		throw DetailedException("Ошибка тестирования. Четвертое поле таблицы PARAMS не ATTRIBUTES")
			.add_detail("Поле", table_params->getfield(3)->getname());
	}

	if (CompareIC(table_params->getfield(4)->getname(), "DATASIZE"))
	{
		throw DetailedException("Ошибка тестирования. Пятое поле таблицы PARAMS не DATASIZE")
			.add_detail("Поле", table_params->getfield(4)->getname());
	}

	if (CompareIC(table_params->getfield(5)->getname(), "BINARYDATA"))
	{
		throw DetailedException("Ошибка тестирования. Шестое поле таблицы PARAMS не BINARYDATA")
			.add_detail("Поле", table_params->getfield(5)->getname());
	}

	if(table_params->get_numfields() > 6)
	{
		if (CompareIC(table_params->getfield(6)->getname(), "PARTNO"))
		{
			throw DetailedException("Ошибка тестирования. Седьмое поле таблицы PARAMS не PARTNO")
				.add_detail("Поле", table_params->getfield(6)->getname());
		}
	}

	// FILES
	if(!table_files)
	{
		throw DetailedException("Ошибка тестирования. В базе нет таблицы FILES");
	}

	if(table_files->get_numfields() < 6)
	{
		throw DetailedException("Ошибка тестирования. В таблице FILES меньше 6 полей")
			.add_detail("Кол-во полей", table_files->get_numfields());
		return false;
	}

	if(table_files->get_numfields() > 7)
	{
		throw DetailedException("Ошибка тестирования. В таблице FILES больше 7 полей")
			.add_detail("Кол-во полей", table_files->get_numfields());
		return false;
	}

	if (CompareIC(table_files->getfield(0)->getname(), "FILENAME"))
	{
		throw DetailedException("Ошибка тестирования. Первое поле таблицы FILES не FILENAME")
			.add_detail("Поле", table_files->getfield(0)->getname());
	}

	if (CompareIC(table_files->getfield(1)->getname(), "CREATION"))
	{
		throw DetailedException("Ошибка тестирования. Второе поле таблицы FILES не CREATION")
			.add_detail("Поле", table_files->getfield(1)->getname());
	}

	if (CompareIC(table_files->getfield(2)->getname(), "MODIFIED"))
	{
		throw DetailedException("Ошибка тестирования. Третье поле таблицы FILES не MODIFIED")
			.add_detail("Поле", table_files->getfield(2)->getname());
	}

	if (CompareIC(table_files->getfield(3)->getname(), "ATTRIBUTES"))
	{
		throw DetailedException("Ошибка тестирования. Четвертое поле таблицы FILES не ATTRIBUTES")
			.add_detail("Поле", table_files->getfield(3)->getname());
	}

	if (CompareIC(table_files->getfield(4)->getname(), "DATASIZE"))
	{
		throw DetailedException("Ошибка тестирования. Пятое поле таблицы FILES не DATASIZE")
			.add_detail("Поле", table_files->getfield(4)->getname());
	}

	if (CompareIC(table_files->getfield(5)->getname(), "BINARYDATA"))
	{
		throw DetailedException("Ошибка тестирования. Шестое поле таблицы FILES не BINARYDATA")
			.add_detail("Поле", table_files->getfield(5)->getname());
	}

	if(table_files->get_numfields() > 6)
	{
		if (CompareIC(table_files->getfield(6)->getname(), "PARTNO"))
		{
			throw DetailedException("Ошибка тестирования. Седьмое поле таблицы FILES не PARTNO")
				.add_detail("Поле", table_files->getfield(6)->getname());
		}
	}

	// DBSCHEMA
	if(!table_dbschema)
	{
		throw DetailedException("Ошибка тестирования. В базе нет таблицы DBSCHEMA");
	}

	if(table_dbschema->get_numfields() != 1)
	{
		throw DetailedException("Ошибка тестирования. В таблице DBSCHEMA не 1 поле")
			.add_detail("Кол-во полей", table_dbschema->get_numfields());
	}

	if (CompareIC(table_dbschema->getfield(0)->getname(), "SERIALIZEDDATA"))
	{
		throw DetailedException("Ошибка тестирования. Первое поле таблицы DBSCHEMA не SERIALIZEDDATA")
			.add_detail("Поле", table_dbschema->getfield(0)->getname());
	}

	//================
	result = true;

	for(i = 0; i < table_config->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format(table_config, i);
		result = result && res;
	}

	for(i = 0; i < table_configsave->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format(table_configsave, i);
		result = result && res;
	}

	for(i = 0; i < table_params->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format(table_params, i);
		result = result && res;
	}

	if(table_dbschema->get_phys_numrecords() < 2)
	{
		throw DetailedException("Ошибка тестирования. В таблице DBSCHEMA нет записей");
	}
	for(i = 0; i < table_dbschema->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format2(table_dbschema, i);
		result = result && res;
	}

	msreg_m.Status("");
	return result;
}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(Table* t, uint32_t nrec)
{
	int32_t j;
	String slen;
	TStream* str;
	bool result;
	bool res;

	TableRecord *rec = t->getrecord(nrec);
	if (rec->is_removed())
	{
		delete rec;
		return true;
	}

	Field *f_name = t->getfield(0);
	Field *f_data_size = t->getfield(4);
	Field *f_binary_data = t->getfield(5);

	std::string path = t->getname() + "/" + rec->get_string(f_name);

	const char *orec = rec->get_raw(f_binary_data);
	if(*(uint32_t*)(orec + 4) > 10 * 1024 * 1024) str = new TTempStream;
	else str = new TMemoryStream();
	t->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

	result = true;
	slen = rec->get_string(f_data_size);
	int file_size;
	try
	{
		file_size = slen.ToInt();
	}
	catch(...)
	{
		throw DetailedException("Ошибка чтения длины файла")
			.add_detail("Путь", path)
			.add_detail("Длина файла", slen);
	}
	if ((int64_t) file_size != str->GetSize()) {
		throw DetailedException("Фактическая длина файла отличается от указанной в таблице")
				.add_detail("Путь", path)
				.add_detail("Фактическая длина файла", str->GetSize())
				.add_detail("Указанная длина файла", slen);
	}

	res = recursive_test_stream_format(str, path, rec->get_string(f_name).size() > GUID_LEN*2); // вторично упакованы могут быть только конфигурации поставщика (файлы с длиной имени более 72 символов)
	result = result && res;

	delete rec;
	delete str;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format2(Table* t, uint32_t nrec)
{
	String path;
	TMemoryStream* str;
	bool result;

	TableRecord *rec = t->getrecord(nrec);
	if (rec->is_removed()) {
		delete rec;
		return true;
	}

	Field *f_sd = t->getfield(0);

	path = t->getname();

	auto bp = rec->get<table_blob_file>(f_sd);
	str = new TMemoryStream();
	t->readBlob(str, bp.blob_start, bp.blob_length);

	result = recursive_test_stream_format(str, path);

	delete rec;
	delete str;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(TStream *str, const string &path, bool maybezipped2)
{
	bool zipped1;
	bool zipped2;
	char _buf[12];
	TStream* _s;
	TStream* _s2;
	TBytesStream* _sb;
	bool result;
	TEncoding *enc;
	std::vector<uint8_t> bytes1;
	std::vector<uint8_t> bytes2;
	V8Catalog* cat;
	int32_t offset;
	String sf;
	wchar_t first_symbol;
	int32_t i;
	bool usetempfile;

	msreg_m.Status(path);

	if(path == "PARAMS/users.usr") return true;

	if(str->GetSize() == 0) return true;
	usetempfile = str->GetSize() > 10 * 1024 * 1024;

	if(usetempfile) _s = new TTempStream;
	else _s = new TMemoryStream();
	zipped1 = false;
	zipped2 = false;

	if(usetempfile) _s2 = new TTempStream;
	else _s2 = new TMemoryStream();

	_s2->CopyFrom(str, 0);
	try
	{
		_s2->Seek(0, soFromBeginning);
		_s->SetSize(0);
		ZInflateStream(_s2, _s);
		zipped1 = true;
	}
	catch (...)
	{
		_s->SetSize(0);
		_s->CopyFrom(str, 0);
	}

	if(zipped1 && maybezipped2)
	{
		_s2->SetSize(0);
		_s2->CopyFrom(_s, 0);
		try
		{
			_s2->Seek(0, soFromBeginning);
			_s->SetSize(0);
			ZInflateStream(_s2, _s);
			zipped2 = true;
		}
		catch (...)
		{
			_s->SetSize(0);
			_s->CopyFrom(_s2, 0);
		}
	}


	try
	{
		cat = new V8Catalog(_s, zipped2, true);
	}
	catch (...)
	{
		throw DetailedException("Ошибка тестирования. Ошибка чтения формата.")
			.add_detail("Путь", path);
	}

	if(!cat || !cat->GetFirst())
	{

		if(_s->GetSize() >= 16)
		{
			_s->Seek(0, soFromBeginning);
			_s->ReadBuffer(_buf, 8);
			if(memcmp(_buf, SIG_MOXCEL, 7) == 0)
			{
				_s->Seek(13, soFromBeginning);
				_s2->SetSize(0);
				_s2->CopyFrom(_s, _s->GetSize() - 13);
				_s->SetSize(0);
				_s->CopyFrom(_s2, 0);
			}
			else if(memcmp(_buf, SIG_SKD, 8) == 0)
			{
				_s->Seek(24, soFromBeginning);
				_s2->SetSize(0);
				_s2->CopyFrom(_s, _s->GetSize() - 24);
				_s->SetSize(0);
				_s->CopyFrom(_s2, 0);
			}
		}

		_sb = new TBytesStream(bytes1);
		_sb->CopyFrom(_s, 0);

		enc = nullptr;
		offset = TEncoding::GetBufferEncoding(_sb->GetBytes(), enc);
		if(offset == 0)
		{
			throw DetailedException("Ошибка тестирования. Ошибка определения кодировки файла")
				.add_detail("Путь", path);
		}

		if(_sb->GetSize()-offset > 0)
		{
			bytes2 = TEncoding::Convert(enc, TEncoding::Unicode, _sb->GetBytes(), offset, _sb->GetSize()-offset);
			if(bytes2.size() == 0)
			{
				throw DetailedException("Ошибка тестирования. Ошибка конвертации")
					.add_detail("Путь", path);
			}


			string sf = String((WCHART*)&bytes2[0], bytes2.size() / 2);
			for(i = 0; i < sf.size(); i++)
			{
				first_symbol = sf[i];
				if (first_symbol != '\r'
					&& first_symbol != '\n'
					&& first_symbol != '\t'
					&& first_symbol != ' ') {
					break;
				}
			}
			if (first_symbol == '{' && !EqualIC(sf.substr(i, 15), "{ХАРАКТЕРИСТИКИ")) {
				tree* rt = parse_1Ctext(sf, path);
				if(rt)
				{
					result = true;
					delete rt;
				}
				else result = false;
			}
			else result = true;

		}
		else result = true;


		delete _sb;

	}
	else result = recursive_test_stream_format(cat, path);

	delete cat;

	delete _s;
	delete _s2;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(V8Catalog *cat, const string &path)
{
	V8Catalog* c;
	bool result;

	V8File* v8f;
	V8File* v8fp;
	String fname;

	result = true;
	v8f = cat->GetFirst();
	while(v8f)
	{
		try
		{
			c = v8f->GetCatalog();
		}
		catch(...)
		{
			throw DetailedException("Ошибка тестирования. Ошибка чтения формата.")
				.add_detail("Путь", path);
		}
		if (c) {
			result = recursive_test_stream_format(c, path + "/" + v8f->GetFileName());
		} else
		{
			fname = v8f->GetFileName();
			if(fname != "module" && fname != "text")
			{
				result = recursive_test_stream_format(v8f->get_stream(), path + "/" + v8f->GetFileName());
			}
		}
		v8fp = v8f;
		v8f = v8f->GetNext();
		v8fp->Close();
	}
	return result;
}

string from_unicode_stream(TStream &f)
{
	vector<uint8_t> buf;
	f.Read(buf, 0);
	return TEncoding::Unicode->toUtf8(buf);
}

//---------------------------------------------------------------------------
bool T_1CD::create_table(const string &path)
{
	TFileStream* f;
	bool fopen;
	String str;
	uint32_t i;
	int32_t j;
	export_import_table_root* root;
	v8ob* ob;
	v8object* descr_table;
	v8object* file_data;
	v8object* file_blob;
	v8object* file_index;
	tree* t;

	boost::filesystem::path dir(path);
	if(!directory_exists(dir)) {
		return false;
	}

	boost::filesystem::path path_root = dir / "root";
	try
	{
		f = new TFileStream(path_root, fmOpenRead);
	}
	catch(...)
	{
		throw DetailedException("Ошибка открытия файла импорта таблицы root")
			.add_detail("Файл", path_root.string());
	}
	root = new export_import_table_root;
	f->Read(root, sizeof(export_import_table_root));
	delete f;


	boost::filesystem::path path_descr = dir / "descr";
	try
	{
		f = new TFileStream(path_descr, fmOpenRead);
	}
	catch(...)
	{
		throw DetailedException("Ошибка открытия файла импорта таблицы descr")
			.add_detail("Файл", path_descr.string());
	}

	string descr_data = from_unicode_stream(*f);
	delete f;

	t = parse_1Ctext(descr_data, path_descr.string());
	string table_name = (*t)[0][0].get_value();

	for(j = 0; j < num_tables; j++) {
		if (EqualIC(tables[j]->getname(), table_name)) {
			delete_table(tables[j]);
		}
	}

	file_data = nullptr;
	file_blob = nullptr;
	file_index = nullptr;

	if(root->has_data)
	{
		boost::filesystem::path path_data = dir / "data";
		try
		{
			f = new TFileStream(path_data, fmOpenRead);
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы data")
				.add_detail("Файл", path_data.string());
		}
		file_data = new v8object(this);
		file_data->setdata(f);
		ob = (v8ob*)getblock_for_write(file_data->get_block_number(), true);
		ob->version.version_1 = root->data_version_1;
		ob->version.version_2 = root->data_version_2;
		delete f;
	}

	if(root->has_blob)
	{
		boost::filesystem::path path_blob = dir / "blob";
		try
		{
			f = new TFileStream(path_blob, fmOpenRead);
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы blob")
				.add_detail("Файл", path_blob.string());
		}
		file_blob = new v8object(this);
		file_blob->setdata(f);
		ob = (v8ob*)getblock_for_write(file_blob->get_block_number(), true);
		ob->version.version_1 = root->blob_version_1;
		ob->version.version_2 = root->blob_version_2;
		delete f;
	}

	if(root->has_index)
	{
		boost::filesystem::path path_index = dir / "index";
		try
		{
			f = new TFileStream(path_index, fmOpenRead);
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы index")
				.add_detail("Файл", path_index.string());
		}
		file_index = new v8object(this);
		file_index->setdata(f);
		ob = (v8ob*)getblock_for_write(file_index->get_block_number(), true);
		ob->version.version_1 = root->index_version_1;
		ob->version.version_2 = root->index_version_2;
		delete f;
	}

	if(root->has_descr)
	{
		try
		{
			f = new TFileStream(path_descr, fmOpenRead);
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы descr")
				.add_detail("Файл", path_descr.string());
		}
		if(fopen)
		{
			descr_table = new v8object(this);
			ob = (v8ob*)getblock_for_write(descr_table->get_block_number(), true);
			ob->version.version_1 = root->descr_version_1;
			ob->version.version_2 = root->descr_version_2;

			string descr = from_unicode_stream(*f);
			delete f;

			auto pos_files = descr.find("{\"Files\",");
			if (pos_files == string::npos) {
				throw DetailedException("Ошибка поиска раздела Files в файле импорта таблицы descr")
					.add_detail("Файл", path_descr.string());
			}
			descr.resize(pos_files);
			descr += "{\"Files\",";
			descr += file_data ? String(file_data->get_block_number()) : String("0");
			descr += ",";
			descr += file_blob ? String(file_blob->get_block_number()) : String("0");
			descr += ",";
			descr += file_index ? String(file_index->get_block_number()) : String("0");
			descr += "}\n}";
			descr_table->setdata(descr.c_str(), descr.size() * 2);

			i = root_object->getlen();
			char *buf = new char[i + 4];
			root_object->getdata(buf, 0, i);

			if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0)
			{
				root_80* root80 = (root_80*)buf;
				root80->blocks[root80->numblocks] = descr_table->get_block_number();
				root80->numblocks++;
				root_object->setdata(buf, i + 4);
			}
			else
			{
				root_81* root81 = (root_81*)buf;
				root81->blocks[root81->numblocks] = descr_table->get_block_number();
				root81->numblocks++;
				root_object->setdata(buf, i + 4);
			}
		}
	}

	flush();

	msreg_m.AddMessage("Таблица создана и импортирована", MessageState::Succesfull)
			.with("Путь", dir.string());

	delete root;
	return true;
}

//---------------------------------------------------------------------------
void T_1CD::set_readonly(bool ro)
{
	readonly = ro;
}

//---------------------------------------------------------------------------
bool T_1CD::test_list_of_tables()
{
	char* rec;
	Field* f_name;
	Field* f_data_size;
	Field* f_binary_data;
	bool hasDBNames;
	bool result;
	bool is_slave;
	String slen;
	String sf;
	TMemoryStream* str;
	TBytesStream* _sb;
	TEncoding *enc;
	std::vector<uint8_t> bytes1;
	std::vector<uint8_t> bytes2;
	wchar_t first_symbol;
	int32_t i, j, l, l2;
	uint32_t k;
	int32_t offset;
	tree* t;
	tree* firstt;

	String _guid;
	String _name;
	String _num;
	String _tabname;

	if(!table_params)
	{
		throw DetailedException("Ошибка тестирования. В базе нет таблицы PARAMS");
		return false;
	}

	if(table_params->get_numfields() < 6)
	{
		throw DetailedException("Ошибка тестирования. В таблице PARAMS меньше 6 полей")
			.add_detail("Кол-во полей", table_params->get_numfields());
	}

	if(table_params->get_numfields() > 7)
	{
		throw DetailedException("Ошибка тестирования. В таблице PARAMS больше 7 полей")
			.add_detail("Кол-во полей", table_params->get_numfields());
	}

	if (CompareIC(table_params->getfield(0)->getname(), "FILENAME"))
	{
		throw DetailedException("Ошибка тестирования. Первое поле таблицы PARAMS не FILENAME")
			.add_detail("Поле", table_params->getfield(0)->getname());
	}

	if (CompareIC(table_params->getfield(1)->getname(), "CREATION"))
	{
		throw DetailedException("Ошибка тестирования. Второе поле таблицы PARAMS не CREATION")
			.add_detail("Поле", table_params->getfield(1)->getname());
	}

	if (CompareIC(table_params->getfield(2)->getname(), "MODIFIED"))
	{
		throw DetailedException("Ошибка тестирования. Третье поле таблицы PARAMS не MODIFIED")
			.add_detail("Поле", table_params->getfield(2)->getname());
	}

	if (CompareIC(table_params->getfield(3)->getname(), "ATTRIBUTES"))
	{
		throw DetailedException("Ошибка тестирования. Четвертое поле таблицы PARAMS не ATTRIBUTES")
			.add_detail("Поле", table_params->getfield(3)->getname());
	}

	if (CompareIC(table_params->getfield(4)->getname(), "DATASIZE"))
	{
		throw DetailedException("Ошибка тестирования. Пятое поле таблицы PARAMS не DATASIZE")
			.add_detail("Поле", table_params->getfield(4)->getname());
	}

	if (CompareIC(table_params->getfield(5)->getname(), "BINARYDATA"))
	{
		throw DetailedException("Ошибка тестирования. Шестое поле таблицы PARAMS не BINARYDATA")
			.add_detail("Поле", table_params->getfield(5)->getname());
	}

	if(table_params->get_numfields() > 6)
	{
		if (CompareIC(table_params->getfield(6)->getname(), "PARTNO"))
		{
			throw DetailedException("Ошибка тестирования. Седьмое поле таблицы PARAMS не PARTNO")
				.add_detail("Поле", table_params->getfield(6)->getname());
		}
	}

	result = true;

	f_name = table_params->getfield(0);
	f_data_size = table_params->getfield(4);
	f_binary_data = table_params->getfield(5);
	rec = new char[table_params->get_recordlen()];

	hasDBNames = false;
	for(k = 0; k < table_params->get_phys_numrecords(); k++)
	{

		TableRecord *rec = table_params->getrecord(k);
		if (rec->is_removed()) {
			continue;
		}

		if (!EqualIC(rec->get_string(f_name), "DBNames")) {
			continue;
		}

		hasDBNames = true;

		auto bp = rec->get<table_blob_file>(f_binary_data);
		str = new TMemoryStream();
		table_params->readBlob(str, bp.blob_start, bp.blob_length);

		string slen = rec->get_string(f_data_size);
		try
		{
			j = std::stoi(slen);
		}
		catch(...)
		{
			throw DetailedException("Ошибка чтения длины файла")
				.add_detail("Путь", "PARAMS/DBNames")
				.add_detail("Длина файла", slen);
		}
		if((int64_t)j != str->GetSize())
		{
			throw DetailedException("Фактическая длина файла отличается от указанной в таблице")
				.add_detail("Путь", "PARAMS/DBNames")
				.add_detail("Фактическая длина файла", str->GetSize())
				.add_detail("Указанная длина файла", slen);
		}

		str->Seek(0, soFromBeginning);
		_sb = new TBytesStream(bytes1);

		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0)
		{
			_sb->CopyFrom(str, 0);
		}
		else
		{
			try
			{
				ZInflateStream(str, _sb);
			}
			catch (...)
			{
				throw DetailedException("Ошибка распаковки данных файла PARAMS/DBNames");
			}
		}
		delete str;

		enc = nullptr;
		offset = TEncoding::GetBufferEncoding(_sb->GetBytes(), enc);
		if(offset == 0)
		{
			throw DetailedException("Ошибка тестирования. Ошибка определения кодировки файла PARAMS/DBNames");
		}
		{
			if(_sb->GetSize()-offset > 0)
			{
				bytes2 = TEncoding::Convert(enc, TEncoding::Unicode, _sb->GetBytes(), offset, _sb->GetSize()-offset);
				if(bytes2.size() == 0)
				{
					throw DetailedException("Ошибка тестирования. Ошибка конвертации файла PARAMS/DBNames");
				}
				{
					sf = String((WCHART*)&bytes2[0], bytes2.size() / 2);
					for(i = 1; i <= sf.size(); i++)
					{
						first_symbol = sf[i];
						if(first_symbol != L'\r' && first_symbol != L'\n' && first_symbol != L'\t' && first_symbol != L' ') break;
					}
					if(first_symbol == L'{')
					{
						tree* rt = parse_1Ctext(sf, "PARAMS/DBNames");
						if(rt)
						{
							firstt = &((*rt)[0][1][1]);

							for(t = firstt; t; t = t->get_next())
							{
								is_slave = false;
								_name = t->get_subnode(1)->get_value();
								if(EqualIC(_name, "Fld")) continue;
								if(EqualIC(_name, "LineNo")) continue;
								if(EqualIC(_name, "Turnover")) continue;
								if(EqualIC(_name, "TurnoverDt")) continue;
								if(EqualIC(_name, "TurnoverCt")) continue;
								if(EqualIC(_name, "ByField")) continue;
								if(EqualIC(_name, "ByOwnerField")) continue;
								if(EqualIC(_name, "ByParentField")) continue;
								if(EqualIC(_name, "ByProperty")) continue;
								if(EqualIC(_name, "ByPropRecorder")) continue;
								if(EqualIC(_name, "ByResource")) continue;
								if(EqualIC(_name, "ByDim")) continue;
								if(EqualIC(_name, "ByDims")) continue;
								if(EqualIC(_name, "ByDimension")) continue;
								if(EqualIC(_name, "ByDimensions")) continue;
								if(EqualIC(_name, "ByDimRecorder")) continue;
								if(EqualIC(_name, "VT")) is_slave = true;
								if(EqualIC(_name, "ExtDim")) is_slave = true;

								_guid = t->get_subnode(0)->get_value();
								_num = t->get_subnode(2)->get_value();

								if(_guid == "00000000-0000-0000-0000-000000000000") continue;

								_tabname = "_";
								_tabname += _name;
								_tabname += _num;
								l = _tabname.size();

								bool table_found = false;
								for(i = 0; i < get_numtables(); i++)
								{
									if(is_slave)
									{
										std::string sf = gettable(i)->getname();
										if (EndsWithIC(sf, _tabname)) {
											table_found = true;
											break;
										}
									}
									else if (EqualIC(gettable(i)->getname(), _tabname))
									{
										table_found = true;
										break;
									}
								}


								if(!table_found)
								{
									throw DetailedException("Отсутствует таблица")
										.add_detail("Имя таблицы", _tabname);
								}
							}

							delete rt;
						}
						else result = false;
					}
					else
					{
						throw DetailedException("Ошибка тестирования. Ошибка разбора файла PARAMS/DBNames. Первый символ не \"{\".");
					}
				}
			}
			else
			{
				throw DetailedException("Ошибка тестирования. Ошибка определения кодировки файла PARAMS/DBNames");
			}
		}

		delete _sb;

		break;

	}


	if(!hasDBNames)
	{
		throw DetailedException("Ошибка тестирования. В таблице PARAMS не найдена запись DBNames.");
	}

	return result;
}

//---------------------------------------------------------------------------
bool T_1CD::replaceTREF(String mapfile)
{
	vector<int32_t> map; // динамический массив соответствия номеров
	String str;
	Table* t;
	Field* f;
	bool editsave;

	TStringList list;
	list.LoadFromFile(mapfile);

	int max_size = 0;
	for (auto &str : list)
	{
		auto l = str.find("\t");
		if (l == string::npos) {
			continue;
		}
		int j = stoi(str.substr(l + 1, str.size() - l - 1));
		if (max_size < j) {
			max_size = j;
		}
	}

	map.resize(max_size + 1);

	for (auto &str : list) {
		auto l = str.find("\t");
		if (l == string::npos) {
			continue;
		}
		int i = stoi(str.substr(0, l));
		int j = stoi(str.substr(l + 1, str.size() - l - 1));
		map[j] = i;
	}

	for (uint32_t i = 0; i < num_tables; i++)
	{
		t = gettable(i);
		for (uint32_t j = 0; j < t->get_numfields(); j ++)
		{
			f = t->getfield(j);
			str = f->getname();
			if (!EndsWithIC(str, "TREF")) {
				continue;
			}
			if (f->gettype() != type_fields::tf_binary || f->getlength() != 4) {
				continue;
			}
			msreg_m.Status(t->getname() + " : " + f->getname());
			editsave = t->edit;
			t->edit = true;
			TableIterator it(t);
			while (!it.eof()) {
				TableRecord rec = it.current();
				auto ii = reverse_byte_order(rec.get<uint32_t>(f));
				if (ii == 0) {
					continue;
				}
				ii = map[ii];
				rec.set<uint32_t>(f, reverse_byte_order(ii));
				// t->write_data_record(kk, rec); // TODO: выпилен кусок кода
			}
			t->edit = editsave;

		}
	}
	msreg_m.Status("");

	flush();

	return true;
}

//---------------------------------------------------------------------------
bool T_1CD::delete_table(Table* tab)
{
	int32_t i;
	uint32_t j;
	uint32_t bl;
	char* buf;
	bool res = num_tables > 0;

	bl = tab->descr_table->block;

	res = res && delete_object(tab->descr_table);
	if(res) res = res && delete_object(tab->file_data);
	if(res) res = res && delete_object(tab->file_blob);
	if(res) res = res && delete_object(tab->file_index);

	if(res)
	{
		tab->descr_table = nullptr;
		tab->file_data = nullptr;
		tab->file_blob = nullptr;
		tab->file_index = nullptr;


		for(i = 0; i < num_tables; i++) if(tables[i] == tab) break;
		num_tables--;
		for(; i < num_tables; i++) tables[i] = tables[i + 1];
		delete tab;

		j = root_object->getlen();
		buf = new char[j];
		root_object->getdata(buf, 0, j);

		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0)
		{
			root_80* root80 = (root_80*)buf;
			for(i = 0; i < root80->numblocks; i++) if(root80->blocks[i] == bl) break;
			root80->numblocks--;
			for(; i < root80->numblocks; i++) root80->blocks[i] = root80->blocks[i + 1];
		}
		else
		{
			root_81* root81 = (root_81*)buf;
			for(i = 0; i < root81->numblocks; i++) if(root81->blocks[i] == bl) break;
			root81->numblocks--;
			for(; i < root81->numblocks; i++) root81->blocks[i] = root81->blocks[i + 1];
		}
		root_object->setdata(buf, j - 4);
		delete[] buf;

	}

	return res;
}

//---------------------------------------------------------------------------
bool T_1CD::delete_object(v8object* ob)
{
	objtab* b;
	uint32_t i;
	int32_t j;

	if(!ob) return true;

	if(ob->block == 1)
	{
		throw DetailedException("Попытка удаления объекта таблицы свободных блоков")
			.add_detail("Номер блока объекта", ob->block);
	}

	if(ob->block == 2)
	{
		throw DetailedException("Попытка удаления корневого объекта")
			.add_detail("Номер блока объекта", ob->block);
	}

	for(i = 0; i < ob->numblocks; i++)
	{
		b = (objtab*)getblock(ob->blocks[i]);
		for(j = 0; j < b->numblocks; j++) set_block_as_free(b->blocks[j]);
	}
	for(i = 0; i < ob->numblocks; i++) set_block_as_free(ob->blocks[i]);
	for(i = 0; i < ob->numblocks; i++) set_block_as_free(ob->block);

	delete ob;

	return true;

}

//---------------------------------------------------------------------------
void T_1CD::find_and_create_lost_tables()
{
	uint32_t k;
	char buf[8];
	v8object* v8obj = nullptr;
	bool block_is_find;
	vector<uint32_t> losttables;
	
	size_t numlosttables = 0;
	for(uint32_t i = 1; i < length; i++)
	{
		getblock(buf, i, 8);
		if(memcmp(buf, SIG_OBJ, 8) == 0)
		{
			block_is_find = false;
			for(v8obj = v8object::get_first(); v8obj; v8obj = v8obj->get_next())
			{
				if(v8obj->get_block_number() == i)
				{
					block_is_find = true;
					break;
				}
			}
			if(!block_is_find)
			{
				v8obj = new v8object(this, i);
				if(v8obj->len > 3)
				{
					try
					{
						v8obj->getdata(buf, 0, 4);
						if(memcmp(buf, SIG_TABDESCR, 4) == 0)
						{
							if(losttables.size() <= numlosttables) losttables.resize(losttables.size() + 1024);
							losttables[numlosttables++] = i;
						}
					}
					catch(...)
					{ //-V565

					}
				}
				delete v8obj;
			}
		}
	}

	if(numlosttables)
	{
		uint64_t root_object_len = root_object->getlen();
		char* b = new char[root_object_len + numlosttables * 4];
		root_object->getdata(b, 0, root_object_len);

		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0)
		{
			root_80* root80 = (root_80*)b;
			for(int32_t j = 0, k = root80->numblocks; j < numlosttables; j++, k++) root80->blocks[k] = losttables[j];
			root80->numblocks += numlosttables;
		}
		else
		{
			root_81* root81 = (root_81*)b;
			for(int32_t j = 0, k = root81->numblocks; j < numlosttables; j++, k++) root81->blocks[k] = losttables[j];
			root81->numblocks += numlosttables;
		}
		root_object->setdata(b, root_object_len + numlosttables * 4);
		delete[] b;

	}

	msreg_m.AddMessage("Поиск и восстановление потерянных таблиц завершены", MessageState::Succesfull)
			.with("Количество восстановленных таблиц", numlosttables);

}

//---------------------------------------------------------------------------
void T_1CD::find_and_save_lost_objects(boost::filesystem::path &lost_objects)
{
	for(uint32_t i = 1; i < length; i++) {
		char buf[8];
		getblock(buf, i, 8);
		if(memcmp(buf, SIG_OBJ, 8) == 0) {
			bool block_is_find = false;
			for(auto v8obj = v8object::get_first(); v8obj; v8obj = v8obj->get_next()) {
				if(v8obj->get_block_number() == i) {
					block_is_find = true;
					break;
				}
			}
			if(!block_is_find) {
				unique_ptr<v8object> find_v8obj(new v8object(this, i));
				find_v8obj->savetofile(lost_objects.string() + "block" + to_string(i));
			}
		}
	}
	msreg_m.AddMessage("Поиск и сохранение потерянных объектов завершен", MessageState::Succesfull);

}

//---------------------------------------------------------------------------
// Если не удалось получить версию, возвращается 0, иначе возвращается положительное число
int32_t T_1CD::get_ver_depot_config(int32_t ver) // Получение номера версии конфигурации (0 - последняя, -1 - предпоследняя и т.д.)
{
	if (ver > 0) {
		return ver;
	}

	assert_i_am_a_repository();

	Index *ind = table_versions->get_index("PK");

	uint32_t i = ind->get_numrecords();
	if (i <= (uint32_t)(-ver)) {
		DetailedException error("Запрошенной версии конфигурации не существует");
		error.add_detail("Всего версий в хранилище", i);
		error.add_detail("Запрошенный номер версии", ver);
		throw error;
	}
	i = ind->get_numrec(i + ver - 1);

	TableRecord *rec = table_versions->getrecord(i);
	String version_presentation = rec->get_string("VERNUM");
	delete rec;

	int32_t version = ToIntDef(version_presentation, 0);
	if (!version) {
		DetailedException error("Не удалось получить реальный номер версии запрошенной конфигурации.");
		error.add_detail("Запрошенный номер версии", ver);
		throw error;
	}

	return version;
}

void T_1CD::assert_i_am_a_repository()
{
	if (!is_open()) {
		throw DetailedException("База не открыта.");
	}

	if (!is_depot) {
		throw DetailedException("База не является хранилищем конфигурации.");
	}

	if (!table_depot) {
		throw DetailedException("В базе хранилища отсутствует таблица DEPOT.");
	}

	if (!table_versions) {
		throw DetailedException("В базе хранилища отсутствует таблица VERSIONS.");
	}

	if (!table_history) {
		throw DetailedException("В базе хранилища отсутствует таблица HISTORY.");
	}

	if (!table_externals) {
		throw DetailedException("В базе хранилища отсутствует таблица EXTERNALS.");
	}
}

//---------------------------------------------------------------------------
// Проверка и восстановление таблицы размещения файла DATA переденной таблицы
// Проверка записей происходит по тестовому шаблону, созданному из описания полей
// Проверяются страницы файла DATA, если проверка не проходит, производится поиск подходящей страницы в файле.
void T_1CD::restore_DATA_allocation_table(Table* tab)
{
	char* rectt;
	uint32_t block;
	v8ob* rootobj;
	objtab* ca;

	uint32_t i, l, cl, a, d, r;
	int32_t j, k, m, n, rl;
	bool ok;
	std::vector<uint32_t> bk;
	String s;

	block = tab->get_file_data()->get_block_number();

	if(block < 5 || block >= length)
	{
		throw DetailedException("Номер корневого блока файла DATA некорректный")
			.add_detail("Таблица", tab->getname())
			.add_detail("Номер блока", block);
	}

	rootobj = (v8ob*)getblock_for_write(block, true);

	if(memcmp(rootobj->sig, SIG_OBJ, 8))
	{
		throw DetailedException("Сигнатура корневого блока файла DATA некорректная.")
			.add_detail("Таблица", tab->getname())
			.add_detail("Номер блока (dec)", block)
			.add_detail("Номер блока (hex)", to_hex_string(block));
	}

	l = rootobj->len;
	rl = tab->get_recordlen();
	if(l / rl * rl != l)
	{
		throw DetailedException("Длина файла DATA не кратна длине одной записи.")
			.add_detail("Таблица", tab->getname())
			.add_detail("Номер блока (dec)", block)
			.add_detail("Номер блока (hex)", to_hex_string(block))
			.add_detail("Длина файла", l)
			.add_detail("Длина записи", tab->get_recordlen());
	}

	rectt = tab->get_record_template_test();

	for(i = 0, j = 0, k = 0; l; ++i)
	{
		cl = std::min(DEFAULT_PAGE_SIZE, l); // TODO работа с pagesize

		if(!j)
		{
			a = rootobj->blocks[k];
			if(a < 5 || a >= length)
			{
				msreg_m.AddMessage("Некорректный номер блока таблицы размещения файла DATA. Создана новая страница размещения", MessageState::Warning)
					.with("Таблица", tab->getname())
					.with("Индекс страницы", k)
					.with("Номер блока", a);
				a = length;
				ca = (objtab*)getblock_for_write(a, false);
			}
			else ca = (objtab*)getblock_for_write(a, true);

			n = ((uint64_t)l + 0xfff) >> 12;
			if(n > 1023) n = 1023;
			m = ca->numblocks;

			if(n != m)
			{
				msreg_m.AddMessage("Некорректное число блоков на странице размещения файла DATA. Исправлено.", MessageState::Warning)
					.with("Таблица", tab->getname())
					.with("Номер блока", a)
					.with("Индекс страницы", k)
					.with("Неверное количество блоков", m)
					.with("Верное количество блоков", n);
				ca->numblocks = n;
			}

			++k;
		}

		d = ca->blocks[j];
		ok = true;
		if(d < 5 || d >= length)
		{
			msreg_m.AddMessage("Некорректный номер страницы данных файла DATA.", MessageState::Warning)
				.with("Таблица", tab->getname())
				.with("Номер блока", a)
				.with("Индекс страницы размещения", k - 1)
				.with("Индекс блока на странице", j)
				.with("Неверный номер страницы", d);
			ok = false;
		}
		if(ok)
		{
			ok = test_block_by_template(d, rectt, i, rl, cl);
			if(!ok)
			{
				msreg_m.AddMessage("Cтраница данных файла DATA не подходит по шаблону.", MessageState::Warning)
					.with("Таблица", tab->getname())
					.with("Номер блока", d)
					.with("Индекс страницы размещения", k - 1)
					.with("Индекс блока на странице", j)
					.with("Индекс страницы в файле DATA", i);
			}
		}

		if(!ok)
		{
			bk.clear();
			for(r = 5; r < length; ++r)
			{
				if(test_block_by_template(r, rectt, i, rl, cl)) bk.push_back(r);
			}
			if(bk.size() == 0)
			{
				throw DetailedException("Не удалось найти подходящую страницу данных файла DATA по шаблону.")
					.add_detail("Таблица", tab->getname())
					.add_detail("Индекс страницы размещения", k - 1)
					.add_detail("Индекс блока на странице", j)
					.add_detail("Индекс страницы в файле DATA", i);
			}

			if(bk.size() == 1)
			{
				d = bk[0];
				ca->blocks[j] = d;
				msreg_m.AddMessage("Найдена подходящая страница данных файла DATA. Страница восстановлена", MessageState::Info)
					.with("Таблица", tab->getname())
					.with("Номер блока", d)
					.with("Индекс страницы размещения", k - 1)
					.with("Индекс блока на странице", j)
					.with("Индекс страницы в файле DATA", i);

			}
			else
			{
				string block_list = "";
				for(d = 0; d < bk.size(); ++d)
				{
					if(d > 0) block_list += ", ";
					block_list += to_hex_string(bk[d]);
				}
				msreg_m.AddMessage("Найдено несколько подходящих страниц данных файла DATA.", MessageState::Hint)
					.with("Таблица", tab->getname())
					.with("Список подходящих блоков", block_list)
					.with("Индекс страницы размещения", k - 1)
					.with("Индекс блока на странице", j)
					.with("Индекс страницы в файле DATA", i);
			}
		}

		if(++j >= 1023) // TODO работа с pagesize
		{
			j = 0;
			++a;
		}
		if(l < DEFAULT_PAGE_SIZE) {
			l = 0; // TODO работа с pagesize
		}
		else {
			l-=DEFAULT_PAGE_SIZE; // TODO работа с pagesize
		}
	}


	delete[] rectt;

	flush();
}

//---------------------------------------------------------------------------
// Проверка блока таблицы по шаблону
bool T_1CD::test_block_by_template(uint32_t testblock, char* tt, uint32_t num, int32_t rlen, int32_t len)
{
	unsigned char b[DEFAULT_PAGE_SIZE]; // TODO работа с pagesize
	int32_t i, j;

	fs->Seek((int64_t)testblock << 12, (TSeekOrigin)soFromBeginning);
	fs->ReadBuffer(b, len);

	if(!num)
	{
		if(b[0] != 1) {
			return false;
		}
		for(i = 1; i < 5; ++i) {
			if(b[i] != 0) {
				return false;
			}
		}
		j = 0;
		i = rlen;
	}
	else
	{
		i = (num << 12) / rlen;
		j = (num << 12) - rlen * i;
		i = 0;
	}
	for(; i < len; ++i)
	{
		if(tt[(j << 8) + b[i]] == 0) {
			return false;
		}
		if(++j >= rlen) {
			j = 0;
		}
	}
	for(i = len; i < DEFAULT_PAGE_SIZE; ++i) {
		if(b[i]) {
			return false; // TODO работа с pagesize
		}
	}
	return true;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_config()
{
	if(!_files_config) {
		_files_config = new TableFiles(table_config);
	}
	return _files_config;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_configsave()
{
	if(!_files_configsave) {
		_files_configsave = new TableFiles(table_configsave);
	}
	return _files_configsave;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_params()
{
	if(!_files_params) {
		_files_params = new TableFiles(table_params);
	}
	return _files_params;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_files()
{
	if(!_files_files) {
		_files_files = new TableFiles(table_files);
	}
	return _files_files;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_configcas()
{
	if(!_files_configcas) {
		_files_configcas = new TableFiles(table_configcas);
	}
	return _files_configcas;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_configcassave()
{
	if(!_files_configcassave) {
		_files_configcassave = new TableFiles(table_configcassave);
	}
	return _files_configcassave;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config_ext(const boost::filesystem::path& file_name, const BinaryGuid& uid, const String& hashname)
{
	std::unique_ptr<ConfigStorageTableConfigCasSave> config_save
			( new ConfigStorageTableConfigCasSave(get_files_configcas(), get_files_configcassave(), uid, hashname) );
	if(!config_save->getready()) {
		return false;
	}

	return config_save->save_config(file_name);
}

//---------------------------------------------------------------------------
bool T_1CD::save_config_ext_db(const boost::filesystem::path& file_name, const String& hashname)
{
	std::unique_ptr<ConfigStorageTableConfigCas> config_save
			( new ConfigStorageTableConfigCas(get_files_configcas(), hashname) );
	if(!config_save->getready()) {
		return false;
	}

	return config_save->save_config(file_name);
}

//---------------------------------------------------------------------------
void T_1CD::pagemapfill()
{
	if(pagemap) delete[] pagemap;
	pagemap = new pagemaprec[length];

	pagemap[0].type = pagetype::root;
	pagemap[1].type = pagetype::freeroot;
	pagemap[2].type = pagetype::rootfileroot;

}

//---------------------------------------------------------------------------
String T_1CD::pagemaprec_presentation(pagemaprec& pmr)
{
	switch(pmr.type)
	{
		case pagetype::lost: return String("потерянная страница");
		case pagetype::root: return String("корневая страница базы");
		case pagetype::freeroot: return String("корневая страница таблицы свободных блоков");
		case pagetype::freealloc: return String("страница размещения таблицы свободных блоков номер ") + pmr.number;
		case pagetype::free: return String("свободная страница номер ") + pmr.number;
		case pagetype::rootfileroot: return String("корневая страница корневого файла");
		case pagetype::rootfilealloc: return String("страница размещения корневого файла номер ") + pmr.number;
		case pagetype::rootfile: return String("страница данных корневого файла номер ") + pmr.number;
		case pagetype::descrroot: return String("корневая страница файла descr таблицы ") + tables[pmr.tab]->getname();
		case pagetype::descralloc: return String("страница размещения файла descr таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::descr: return String("страница данных файла descr таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::dataroot: return String("корневая страница файла data таблицы ") + tables[pmr.tab]->getname();
		case pagetype::dataalloc: return String("страница размещения файла data таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::data: return String("страница данных файла data таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::indexroot: return String("корневая страница файла index таблицы ") + tables[pmr.tab]->getname();
		case pagetype::indexalloc: return String("страница размещения файла index таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::index: return String("страница данных файла index таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::blobroot: return String("корневая страница файла blob таблицы ") + tables[pmr.tab]->getname();
		case pagetype::bloballoc: return String("страница размещения файла blob таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pagetype::blob: return String("страница данных файла blob таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;

		default: return String("??? неизвестный тип страницы ???");
	}
}

depot_ver T_1CD::get_depot_version(const TableRecord &record)
{
	std::string Ver = record.get_string("DEPOTVER");

	if (EqualIC(Ver, "0300000000000000")) {
		return depot_ver::Ver3;
	}
	if (EqualIC(Ver, "0500000000000000")) {
		return depot_ver::Ver5;
	}
	if (EqualIC(Ver, "0600000000000000")) {
		return depot_ver::Ver6;
	}
	if (EqualIC(Ver, "0700000000000000")) {
		return depot_ver::Ver7;
	}

	DetailedException error("Неизвестная версия хранилища!");
	error.add_detail("Версия хранилища", Ver);
	throw error;
}

T_1CD::SupplierConfigs& T_1CD::supplier_configs() {
	if(!supplier_configs_defined) {
		find_supplier_configs();
	}

	return _supplier_configs;
}
