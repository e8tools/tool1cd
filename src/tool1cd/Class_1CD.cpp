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
		msreg_m.AddError("Попытка чтения блока за пределами файла.",
			"Индекс блока", to_hex_string(block_number),
			"Всего блоков", to_hex_string(length));
		return false;
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
		msreg_m.AddError("Попытка чтения блока за пределами файла.",
			"Индекс блока", to_hex_string(block_number),
			"Всего блоков", to_hex_string(length));
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
		msreg_m.AddError("Попытка получения блока за пределами файла базы.",
			"Индекс блока", to_hex_string(block_number),
			"Всего блоков", to_hex_string(length));
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
		msreg_m.AddError("Попытка получения таблицы по номеру, превышающему количество таблиц",
			"Количество таблиц", num_tables,
			"Номер таблицы", numtable + 1);
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
	char* b;
	uint32_t* table_blocks;
	int32_t i, j;
	TMemoryStream* tstr;
	root_80* root80;
	root_81* root81;

	msreg_m.AddMessageRegistrator(mess);

	init();

	filename = System::Ioutils::TPath::GetFullPath(_filename);

	try
	{
		if(_monopoly) fs = new TFileStream(filename, fmOpenReadWrite | fmShareDenyWrite);
		else fs = new TFileStream(filename, fmOpenRead | fmShareDenyNone);
	}
	catch(...)
	{
		msreg_m.AddError("Ошибка открытия файла базы (файл открыт другой программой?)");
		fs = nullptr;
		return;
	}

	v8con* cont = new v8con;
	fs->Read(cont, sizeof(v8con));

	if(memcmp(&(cont->sig), SIG_CON, 8) != 0)
	{
		msreg_m.AddError("Файл не является базой 1С (сигнатура не равна \"1CDBMSV8\")");
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
		msreg_m.AddError("Неподдерживаемая версия базы 1С",
			"Версия базы", ver);
		delete fs;
		fs = nullptr;
		delete cont;
		return;
	}

	length = fs->GetSize() / pagesize;
	if((int64_t)length * pagesize != fs->GetSize())
	{
		msreg_m.AddError(String("Длина файла базы не кратна длине страницы (" + to_hex_string(pagesize) + ")"),
			"Длина файла", to_hex_string(fs->GetSize()));
		delete fs;
		fs = nullptr;
		return;
	}

	MemBlock::pagesize = pagesize;
	MemBlock::maxcount = ONE_GB / pagesize; // гигабайт
	MemBlock::create_memblocks(length);

	if(length != cont->length)
	{
		msreg_m.AddError("Длина файла в блоках и количество блоков в заголовке не равны",
			"Длина файла в блоках", length,
			"Блоков в заголовке", cont->length);
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
		if(!tables[j]->getname().CompareIC("CONFIG")) table_config = tables[j];
		if(!tables[j]->getname().CompareIC("CONFIGSAVE")) table_configsave = tables[j];
		if(!tables[j]->getname().CompareIC("PARAMS")) table_params = tables[j];
		if(!tables[j]->getname().CompareIC("FILES")) table_files = tables[j];
		if(!tables[j]->getname().CompareIC("DBSCHEMA")) table_dbschema = tables[j];
		if(!tables[j]->getname().CompareIC("CONFIGCAS")) table_configcas = tables[j];
		if(!tables[j]->getname().CompareIC("CONFIGCASSAVE")) table_configcassave = tables[j];
		if(!tables[j]->getname().CompareIC("_EXTENSIONSINFO")) table__extensionsinfo = tables[j];

		if(!tables[j]->getname().CompareIC("DEPOT")) table_depot = tables[j];
		if(!tables[j]->getname().CompareIC("USERS")) table_users = tables[j];
		if(!tables[j]->getname().CompareIC("OBJECTS")) table_objects = tables[j];
		if(!tables[j]->getname().CompareIC("VERSIONS")) table_versions = tables[j];
		if(!tables[j]->getname().CompareIC("LABELS")) table_labels = tables[j];
		if(!tables[j]->getname().CompareIC("HISTORY")) table_history = tables[j];
		if(!tables[j]->getname().CompareIC("LASTESTVERSIONS")) table_lastestversions = tables[j];
		if(!tables[j]->getname().CompareIC("EXTERNALS")) table_externals = tables[j];
		if(!tables[j]->getname().CompareIC("SELFREFS")) table_selfrefs = tables[j];
		if(!tables[j]->getname().CompareIC("OUTREFS")) table_outrefs = tables[j];

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
	if(!table_config) msreg_m.AddError("Отсутствует таблица CONFIG");
#else
#ifdef delic
	if(!table_params) msreg_m.AddError("Отсутствует таблица PARAMS");
#ifdef delicfiles
	if(!table_config) msreg_m.AddError("Отсутствует таблица FILES");
	if(!table_config) msreg_m.AddError("Отсутствует таблица CONFIG");
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
			if(!table_depot) msreg_m.AddError("Отсутствует таблица DEPOT");
			if(!table_users) msreg_m.AddError("Отсутствует таблица USERS");
			if(!table_objects) msreg_m.AddError("Отсутствует таблица OBJECTS");
			if(!table_versions) msreg_m.AddError("Отсутствует таблица VERSIONS");
			if(!table_labels) msreg_m.AddError("Отсутствует таблица LABELS");
			if(!table_history) msreg_m.AddError("Отсутствует таблица HISTORY");
			if(!table_lastestversions) msreg_m.AddError("Отсутствует таблица LASTESTVERSIONS");
			if(!table_externals) msreg_m.AddError("Отсутствует таблица EXTERNALS");
			if(!table_selfrefs) msreg_m.AddError("Отсутствует таблица SELFREFS");
			if(!table_outrefs) msreg_m.AddError("Отсутствует таблица OUTREFS");
			FieldType::showGUIDasMS = true; // TODO: wat??
		}
	}
	else
	{
		_is_infobase = true;
		if(!table_config) msreg_m.AddError("Отсутствует таблица CONFIG");
		if(!table_configsave) msreg_m.AddError("Отсутствует таблица CONFIGSAVE");
		if(!table_params) msreg_m.AddError("Отсутствует таблица PARAMS");
		if(!table_files) msreg_m.AddError("Отсутствует таблица FILES");
		if(!table_dbschema) msreg_m.AddError("Отсутствует таблица DBSCHEMA");
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
		if(config_save.first.GetLength() == SUPPLIER_CONFIG_NAME_LEN) {
			 add_supplier_config(config_save.second);
		}
	}

	for(auto& config : get_files_config()->files()) {
		if(config.first.GetLength() == SUPPLIER_CONFIG_NAME_LEN) {
			add_supplier_config(config.second);
		}
	}

	supplier_configs_defined = true;
}

//---------------------------------------------------------------------------
void T_1CD::add_supplier_config(TableFile* tf)
{
	ContainerFile* f;
	TStream* s;
	int32_t i;
	v8catalog* cat = nullptr;
	v8catalog* cat2;
	v8file* file;
	tree* tr = nullptr;
	String filenamemeta;
	String nodetype;
	String _name; // имя конфигурация поставщика
	String _supplier; // синоним конфигурация поставщика
	String _version; // версия конфигурация поставщика

	f = new ContainerFile(tf, tf->name);
	if(!f->open())
	{
		delete f;
		return;
	}

	s = new TTempStream;

	try
	{
		try
		{
			ZInflateStream(f->stream, s);
		}
		catch(...)
		{
			msreg_m.AddError("Ошибка распаковки конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя", tf->name);
			delete s;
			return;
		}
		f->close();
		delete f;
		f = nullptr;

		cat = new v8catalog(s, true);
		s = nullptr;
		file = cat->GetFile("version");
		if(!file)
		{
			msreg_m.AddError("Не найден файл version в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name);
			delete cat;
			return;
		}

		tr = file->get_tree();
		i = (*tr)[0][0][0].get_value().ToInt();
		delete tr;
		tr = nullptr;

		#ifdef _DEBUG
		msreg_m.AddDebugMessage("Найдена версия контейнера конфигурации поставщика", MessageState::Info,
			"Таблица", tf->t->getname(),
			"Имя файла", tf->name,
			"Версия", i);
		#endif

		if(i < 100) // 8.0
		{
			file = cat->GetFile("metadata");
			if(!file)
			{
				msreg_m.AddError("Не найден каталог metadata в конфигурации поставщика",
					"Таблица", tf->t->getname(),
					"Имя файла", tf->name);
				delete cat;
				return;
			}
			cat2 = file->GetCatalog();
			if(!cat2)
			{
				msreg_m.AddError("Файл metadata не является каталогом в конфигурации поставщика",
					"Таблица", tf->t->getname(),
					"Имя файла", tf->name);
				delete cat;
				return;
			}

		}
		else cat2 = cat; // else 8.1 или 8.2

		file = cat2->GetFile("root");
		if(!file)
		{
			msreg_m.AddError("Не найден файл root в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name);
			delete cat;
			return;
		}

		tr = file->get_tree();
		filenamemeta = (*tr)[0][1].get_value();
		delete tr;
		tr = nullptr;

		file = cat2->GetFile(filenamemeta);
		if(!file)
		{
			msreg_m.AddError("Не найден файл метаданных в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name,
				"Имя мета", filenamemeta);
			delete cat;
			return;
		}

		#ifdef _DEBUG
		msreg_m.AddDebugMessage("Найден файл метаданных в конфигурации поставщика", MessageState::Info,
			"Таблица", tf->t->getname(),
			"Имя файла", tf->name,
			"Имя мета", filenamemeta);
		#endif

		tr = file->get_tree();
		int32_t numnode = (*tr)[0][2].get_value().ToInt();
		for(i = 0; i < numnode; i++)
		{
			tree& node = (*tr)[0][3 + i];
			nodetype = node[0].get_value();
			if(nodetype.CompareIC(NODE_GENERAL) == 0) // узел "Общие"
			{
				tree& confinfo = node[1][1];
				int32_t verconfinfo = confinfo[0].get_value().ToInt();
				switch(verconfinfo)
				{
					case 15:
						_name = confinfo[1][1][2].get_value();
						_supplier = confinfo[11].get_value();
						_version = confinfo[12].get_value();
						break;
					case 22:
					case 32:
					case 34:
					case 36:
					case 37:
						_name = confinfo[1][1][2].get_value();
						_supplier = confinfo[14].get_value();
						_version = confinfo[15].get_value();
						break;
					default:
						_name = confinfo[1][1][2].get_value();
						_supplier = confinfo[14].get_value();
						_version = confinfo[15].get_value();
						#ifdef _DEBUG
						msreg_m.AddDebugMessage("Неизвестная версия свойств конфигурации поставщика", MessageState::Info,
							"Таблица", tf->t->getname(),
							"Имя файла", tf->name,
							"Имя мета", filenamemeta,
							"Версия свойств", verconfinfo);
						#endif
						break;
				}
				break;
			}
		}
		delete tr;
		tr = nullptr;

		if(i >= numnode)
		{
			msreg_m.AddError("Не найден узел Общие в метаданных конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name,
				"Имя мета", filenamemeta);
			_supplier = "";
			_version = "";
		}
		#ifdef _DEBUG
		else
		{
			msreg_m.AddDebugMessage("Найдена конфигурация поставщика", MessageState::Info,
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name,
				"Имя", _name,
				"Версия", _version,
				"Поставщик", _supplier);
		}
		#endif

		std::shared_ptr<SupplierConfig> sup_conf = std::make_shared<SupplierConfig>(tf, _name, _supplier, _version);
		_supplier_configs.push_back(sup_conf);

		delete cat;
		cat = nullptr;
	}
	catch(...)
	{
		msreg_m.AddError("Произошла ошибка при разборе конфигурации поставщика",
			"Таблица", tf->t->getname(),
			"Имя файла", tf->name);
		delete cat;
		delete s;
		delete tr;
		delete f;
	}
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
			if(!block_is_find) msreg_m.AddMessage_("Найден потерянный объект", MessageState::Info, "Номер блока", to_hex_string(i));
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
		msreg_m.AddError("Ошибка тестирования. В базе нет таблицы CONFIG");
		return false;
	}

	if(table_config->get_numfields() < 6)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице CONFIG меньше 6 полей",
			"Кол-во полей", table_config->get_numfields());
		return false;
	}

	if(table_config->get_numfields() > 7)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице CONFIG больше 7 полей",
			"Кол-во полей", table_config->get_numfields());
		return false;
	}

	if(table_config->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_m.AddError("Ошибка тестирования. Первое поле таблицы CONFIG не FILENAME",
			"Поле", table_config->getfield(0)->getname());
		return false;
	}

	if(table_config->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_m.AddError("Ошибка тестирования. Второе поле таблицы CONFIG не CREATION",
			"Поле", table_config->getfield(1)->getname());
		return false;
	}

	if(table_config->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_m.AddError("Ошибка тестирования. Третье поле таблицы CONFIG не MODIFIED",
			"Поле", table_config->getfield(2)->getname());
		return false;
	}

	if(table_config->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_m.AddError("Ошибка тестирования. Четвертое поле таблицы CONFIG не ATTRIBUTES",
			"Поле", table_config->getfield(3)->getname());
		return false;
	}

	if(table_config->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_m.AddError("Ошибка тестирования. Пятое поле таблицы CONFIG не DATASIZE",
			"Поле", table_config->getfield(4)->getname());
		return false;
	}

	if(table_config->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_m.AddError("Ошибка тестирования. Шестое поле таблицы CONFIG не BINARYDATA",
			"Поле", table_config->getfield(5)->getname());
		return false;
	}

	if(table_config->get_numfields() > 6)
	{
		if(table_config->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_m.AddError("Ошибка тестирования. Седьмое поле таблицы CONFIG не PARTNO",
				"Поле", table_config->getfield(6)->getname());
			return false;
		}
	}

	// CONFIGSAVE
	if(!table_configsave)
	{
		msreg_m.AddError("Ошибка тестирования. В базе нет таблицы CONFIGSAVE");
		return false;
	}

	if(table_configsave->get_numfields() < 6)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице CONFIGSAVE меньше 6 полей",
			"Кол-во полей", table_configsave->get_numfields());
		return false;
	}

	if(table_configsave->get_numfields() > 7)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице CONFIGSAVE больше 7 полей",
			"Кол-во полей", table_configsave->get_numfields());
		return false;
	}

	if(table_configsave->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_m.AddError("Ошибка тестирования. Первое поле таблицы CONFIGSAVE не FILENAME",
			"Поле", table_configsave->getfield(0)->getname());
		return false;
	}

	if(table_configsave->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_m.AddError("Ошибка тестирования. Второе поле таблицы CONFIGSAVE не CREATION",
			"Поле", table_configsave->getfield(1)->getname());
		return false;
	}

	if(table_configsave->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_m.AddError("Ошибка тестирования. Третье поле таблицы CONFIGSAVE не MODIFIED",
			"Поле", table_configsave->getfield(2)->getname());
		return false;
	}

	if(table_configsave->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_m.AddError("Ошибка тестирования. Четвертое поле таблицы CONFIGSAVE не ATTRIBUTES",
			"Поле", table_configsave->getfield(3)->getname());
		return false;
	}

	if(table_configsave->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_m.AddError("Ошибка тестирования. Пятое поле таблицы CONFIGSAVE не DATASIZE",
			"Поле", table_configsave->getfield(4)->getname());
		return false;
	}

	if(table_configsave->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_m.AddError("Ошибка тестирования. Шестое поле таблицы CONFIGSAVE не BINARYDATA",
			"Поле", table_configsave->getfield(5)->getname());
		return false;
	}

	if(table_configsave->get_numfields() > 6)
	{
		if(table_configsave->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_m.AddError("Ошибка тестирования. Седьмое поле таблицы CONFIGSAVE не PARTNO",
				"Поле", table_configsave->getfield(6)->getname());
			return false;
		}
	}

	// PARAMS
	if(!table_params)
	{
		msreg_m.AddError("Ошибка тестирования. В базе нет таблицы PARAMS");
		return false;
	}

	if(table_params->get_numfields() < 6)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице PARAMS меньше 6 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->get_numfields() > 7)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице PARAMS больше 7 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_m.AddError("Ошибка тестирования. Первое поле таблицы PARAMS не FILENAME",
			"Поле", table_params->getfield(0)->getname());
		return false;
	}

	if(table_params->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_m.AddError("Ошибка тестирования. Второе поле таблицы PARAMS не CREATION",
			"Поле", table_params->getfield(1)->getname());
		return false;
	}

	if(table_params->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_m.AddError("Ошибка тестирования. Третье поле таблицы PARAMS не MODIFIED",
			"Поле", table_params->getfield(2)->getname());
		return false;
	}

	if(table_params->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_m.AddError("Ошибка тестирования. Четвертое поле таблицы PARAMS не ATTRIBUTES",
			"Поле", table_params->getfield(3)->getname());
		return false;
	}

	if(table_params->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_m.AddError("Ошибка тестирования. Пятое поле таблицы PARAMS не DATASIZE",
			"Поле", table_params->getfield(4)->getname());
		return false;
	}

	if(table_params->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_m.AddError("Ошибка тестирования. Шестое поле таблицы PARAMS не BINARYDATA",
			"Поле", table_params->getfield(5)->getname());
		return false;
	}

	if(table_params->get_numfields() > 6)
	{
		if(table_params->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_m.AddError("Ошибка тестирования. Седьмое поле таблицы PARAMS не PARTNO",
				"Поле", table_params->getfield(6)->getname());
			return false;
		}
	}

	// FILES
	if(!table_files)
	{
		msreg_m.AddError("Ошибка тестирования. В базе нет таблицы FILES");
		return false;
	}

	if(table_files->get_numfields() < 6)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице FILES меньше 6 полей",
			"Кол-во полей", table_files->get_numfields());
		return false;
	}

	if(table_files->get_numfields() > 7)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице FILES больше 7 полей",
			"Кол-во полей", table_files->get_numfields());
		return false;
	}

	if(table_files->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_m.AddError("Ошибка тестирования. Первое поле таблицы FILES не FILENAME",
			"Поле", table_files->getfield(0)->getname());
		return false;
	}

	if(table_files->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_m.AddError("Ошибка тестирования. Второе поле таблицы FILES не CREATION",
			"Поле", table_files->getfield(1)->getname());
		return false;
	}

	if(table_files->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_m.AddError("Ошибка тестирования. Третье поле таблицы FILES не MODIFIED",
			"Поле", table_files->getfield(2)->getname());
		return false;
	}

	if(table_files->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_m.AddError("Ошибка тестирования. Четвертое поле таблицы FILES не ATTRIBUTES",
			"Поле", table_files->getfield(3)->getname());
		return false;
	}

	if(table_files->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_m.AddError("Ошибка тестирования. Пятое поле таблицы FILES не DATASIZE",
			"Поле", table_files->getfield(4)->getname());
		return false;
	}

	if(table_files->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_m.AddError("Ошибка тестирования. Шестое поле таблицы FILES не BINARYDATA",
			"Поле", table_files->getfield(5)->getname());
		return false;
	}

	if(table_files->get_numfields() > 6)
	{
		if(table_files->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_m.AddError("Ошибка тестирования. Седьмое поле таблицы FILES не PARTNO",
				"Поле", table_files->getfield(6)->getname());
			return false;
		}
	}

	// DBSCHEMA
	if(!table_dbschema)
	{
		msreg_m.AddError("Ошибка тестирования. В базе нет таблицы DBSCHEMA");
		return false;
	}

	if(table_dbschema->get_numfields() != 1)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице DBSCHEMA не 1 поле",
			"Кол-во полей", table_dbschema->get_numfields());
		return false;
	}

	if(table_dbschema->getfield(0)->getname().CompareIC("SERIALIZEDDATA"))
	{
		msreg_m.AddError("Ошибка тестирования. Первое поле таблицы DBSCHEMA не SERIALIZEDDATA",
			"Поле", table_dbschema->getfield(0)->getname());
		return false;
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
		msreg_m.AddError("Ошибка тестирования. В таблице DBSCHEMA нет записей");
		result = false;
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
	String path;
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

	path = t->getname() + "/" + rec->get_string(f_name);

	const char *orec = rec->get_raw(f_binary_data);
	if(*(uint32_t*)(orec + 4) > 10 * 1024 * 1024) str = new TTempStream;
	else str = new TMemoryStream();
	t->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

	result = true;
	slen = rec->get_string(f_data_size);
	try
	{
		j = slen.ToInt();
	}
	catch(...)
	{
		msreg_m.AddMessage_("Ошибка чтения длины файла", MessageState::Warning,
			"Путь", path,
			"Длина файла", slen);
		result = false;
	}
	if(result) if((int64_t)j != str->GetSize())
	{
		msreg_m.AddMessage_("Фактическая длина файла отличается от указанной в таблице", MessageState::Warning,
			"Путь", path,
			"Фактическая длина файла", str->GetSize(),
			"Указанная длина файла", slen);
		result = false;
	}

	res = recursive_test_stream_format(str, path, rec->get_string(f_name).GetLength() > 72); // вторично упакованы могут быть только конфигурации поставщика (файлы с длиной имени более 72 символов)
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

	auto bp = (const table_blob_file *)rec->get_raw(f_sd->getoffset());
	str = new TMemoryStream();
	t->readBlob(str, bp->blob_start, bp->blob_length);

	result = recursive_test_stream_format(str, path);

	delete rec;
	delete str;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(TStream* str, String path, bool maybezipped2)
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
	v8catalog* cat;
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
		cat = new v8catalog(_s, zipped2, true);
	}
	catch (...)
	{
		msreg_m.AddError("Ошибка тестирования. Ошибка чтения формата.",
			"Путь", path);
		cat = nullptr;

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
			msreg_m.AddError("Ошибка тестирования. Ошибка определения кодировки файла",
				"Путь", path);
			result = false;
		}
		else
		{
			if(_sb->GetSize()-offset > 0)
			{
				bytes2 = TEncoding::Convert(enc, TEncoding::Unicode, _sb->GetBytes(), offset, _sb->GetSize()-offset);
				if(bytes2.size() == 0)
				{
					msreg_m.AddError("Ошибка тестирования. Ошибка конвертации",
						"Путь", path);
					result = false;
				}
				else
				{
					sf = String((WCHART*)&bytes2[0], bytes2.size() / 2);
					for(i = 1; i <= sf.GetLength(); i++)
					{
						first_symbol = sf[i];
						if(first_symbol != L'\r' && first_symbol != L'\n' && first_symbol != L'\t' && first_symbol != L' ') break;
					}
					if(first_symbol == L'{' && sf.SubString(i, 15).CompareIC("{ХАРАКТЕРИСТИКИ"))
					{
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
			}
			else result = true;
		}

		delete _sb;

	}
	else result = recursive_test_stream_format(cat, path);

	delete cat;

	delete _s;
	delete _s2;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(v8catalog* cat, String path)
{
	v8catalog* c;
	bool result;

	v8file* v8f;
	v8file* v8fp;
	String fname;

	result = true;
	v8f = cat->GetFirst();
	while(v8f)
	{
		result = false;
		try
		{
			c = v8f->GetCatalog();
			result = true;
		}
		catch(...)
		{
			msreg_m.AddError("Ошибка тестирования. Ошибка чтения формата.",
				"Путь", path);
			c = nullptr;
		}
		if(result)
		{
			if(c) result = recursive_test_stream_format(c, path + "/" + v8f->GetFileName());
			else
			{
				fname = v8f->GetFileName();
				if(fname != "module" && fname != "text")
				{
					result = recursive_test_stream_format(v8f->get_stream(), path + "/" + v8f->GetFileName());
				}
			}
		}
		v8fp = v8f;
		v8f = v8f->GetNext();
		v8fp->Close();
	}
	return result;
}

//---------------------------------------------------------------------------
bool T_1CD::create_table(String path)
{
	TFileStream* f;
	bool fopen;
	String str;
	char* buf;
	uint32_t i;
	int32_t j;
	export_import_table_root* root;
	v8ob* ob;
	v8object* descr_table;
	v8object* file_data;
	v8object* file_blob;
	v8object* file_index;
	tree* t;

	boost::filesystem::path dir(static_cast<std::string>(path));
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
		msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы root", MessageState::Warning,
			"Файл", path_root.string());
		return false;
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
		msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы descr", MessageState::Warning,
			"Файл", path_descr.string());
		return false;
	}

	i = f->GetSize();
	buf = new char[i + 2];
	f->Read(buf, i);
	buf[i] = 0;
	buf[i + 1] = 0;
	str = String((WCHART*)buf);
	delete[] buf;
	delete f;

	t = parse_1Ctext(str, path_descr.string());
	str = (*t)[0][0].get_value();

	for(j = 0; j < num_tables; j++) if(tables[j]->getname().CompareIC(str) == 0)
	{
		delete_table(tables[j]);
	}

	file_data = nullptr;
	file_blob = nullptr;
	file_index = nullptr;

	if(root->has_data)
	{
		fopen = false;
		boost::filesystem::path path_data = dir / "data";
		try
		{
			f = new TFileStream(path_data, fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы data", MessageState::Warning,
				"Файл", path_data.string());
		}
		if(fopen)
		{
			file_data = new v8object(this);
			file_data->setdata(f);
			ob = (v8ob*)getblock_for_write(file_data->get_block_number(), true);
			ob->version.version_1 = root->data_version_1;
			ob->version.version_2 = root->data_version_2;
			delete f;
		}
	}

	if(root->has_blob)
	{
		fopen = false;
		boost::filesystem::path path_blob = dir / "blob";
		try
		{
			f = new TFileStream(path_blob, fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы blob", MessageState::Warning,
				"Файл", path_blob.string());
		}
		if(fopen)
		{
			file_blob = new v8object(this);
			file_blob->setdata(f);
			ob = (v8ob*)getblock_for_write(file_blob->get_block_number(), true);
			ob->version.version_1 = root->blob_version_1;
			ob->version.version_2 = root->blob_version_2;
			delete f;
		}
	}

	if(root->has_index)
	{
		fopen = false;
		boost::filesystem::path path_index = dir / "index";
		try
		{
			f = new TFileStream(path_index, fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы index", MessageState::Warning,
				"Файл", path_index.string());
		}
		if(fopen)
		{
			file_index = new v8object(this);
			file_index->setdata(f);
			ob = (v8ob*)getblock_for_write(file_index->get_block_number(), true);
			ob->version.version_1 = root->index_version_1;
			ob->version.version_2 = root->index_version_2;
			delete f;
		}
	}

	if(root->has_descr)
	{
		fopen = false;
		try
		{
			f = new TFileStream(path_descr, fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы descr", MessageState::Warning,
				"Файл", path_descr.string());
		}
		if(fopen)
		{
			descr_table = new v8object(this);
			ob = (v8ob*)getblock_for_write(descr_table->get_block_number(), true);
			ob->version.version_1 = root->descr_version_1;
			ob->version.version_2 = root->descr_version_2;

			i = f->GetSize();
			buf = new char[i + 2];
			f->Read(buf, i);
			buf[i] =0;
			buf[i + 1] =0;
			str = String((WCHART*)buf);
			delete[] buf;
			delete f;

			i = str.Pos("{\"Files\",");
			if(i == 0)
			{
				msreg_m.AddMessage_("Ошибка поиска раздела Files в файле импорта таблицы descr", MessageState::Warning,
					"Файл", path_descr.string());
				delete root;
				return false;
			}
			str.SetLength(i - 1);
			str += "{\"Files\",";
			str += file_data ? String(file_data->get_block_number()) : String("0");
			str += ",";
			str += file_blob ? String(file_blob->get_block_number()) : String("0");
			str += ",";
			str += file_index ? String(file_index->get_block_number()) : String("0");
			str += "}\n}";
			descr_table->setdata(str.c_str(), str.GetLength() * 2);

			i = root_object->getlen();
			buf = new char[i + 4];
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

	msreg_m.AddMessage_("Таблица создана и импортирована", MessageState::Succesfull,
		"Путь", dir.string());

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
	char* orec;
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
		msreg_m.AddError("Ошибка тестирования. В базе нет таблицы PARAMS");
		return false;
	}

	if(table_params->get_numfields() < 6)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице PARAMS меньше 6 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->get_numfields() > 7)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице PARAMS больше 7 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_m.AddError("Ошибка тестирования. Первое поле таблицы PARAMS не FILENAME",
			"Поле", table_params->getfield(0)->getname());
		return false;
	}

	if(table_params->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_m.AddError("Ошибка тестирования. Второе поле таблицы PARAMS не CREATION",
			"Поле", table_params->getfield(1)->getname());
		return false;
	}

	if(table_params->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_m.AddError("Ошибка тестирования. Третье поле таблицы PARAMS не MODIFIED",
			"Поле", table_params->getfield(2)->getname());
		return false;
	}

	if(table_params->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_m.AddError("Ошибка тестирования. Четвертое поле таблицы PARAMS не ATTRIBUTES",
			"Поле", table_params->getfield(3)->getname());
		return false;
	}

	if(table_params->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_m.AddError("Ошибка тестирования. Пятое поле таблицы PARAMS не DATASIZE",
			"Поле", table_params->getfield(4)->getname());
		return false;
	}

	if(table_params->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_m.AddError("Ошибка тестирования. Шестое поле таблицы PARAMS не BINARYDATA",
			"Поле", table_params->getfield(5)->getname());
		return false;
	}

	if(table_params->get_numfields() > 6)
	{
		if(table_params->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_m.AddError("Ошибка тестирования. Седьмое поле таблицы PARAMS не PARTNO",
				"Поле", table_params->getfield(6)->getname());
			return false;
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

		if (rec->get_string(f_name).CompareIC("DBNames") != 0) {
			continue;
		}

		hasDBNames = true;

		auto bp = (const table_blob_file *) rec->get_data(f_binary_data);
		str = new TMemoryStream();
		table_params->readBlob(str, bp->blob_start, bp->blob_length);

		String slen = rec->get_string(f_data_size);
		try
		{
			j = slen.ToInt();
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка чтения длины файла", MessageState::Warning,
				"Путь", "PARAMS/DBNames",
				"Длина файла", slen);
			result = false;
			break;
		}
		if((int64_t)j != str->GetSize())
		{
			msreg_m.AddMessage_("Фактическая длина файла отличается от указанной в таблице", MessageState::Warning,
				"Путь", "PARAMS/DBNames",
				"Фактическая длина файла", str->GetSize(),
				"Указанная длина файла", slen);
			result = false;
			break;
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
				msreg_m.AddMessage("Ошибка распаковки данных файла PARAMS/DBNames", MessageState::Error);
				result = false;
				break;
			}
		}
		delete str;

		enc = nullptr;
		offset = TEncoding::GetBufferEncoding(_sb->GetBytes(), enc);
		if(offset == 0)
		{
			msreg_m.AddError("Ошибка тестирования. Ошибка определения кодировки файла PARAMS/DBNames");
			result = false;
		}
		else
		{
			if(_sb->GetSize()-offset > 0)
			{
				bytes2 = TEncoding::Convert(enc, TEncoding::Unicode, _sb->GetBytes(), offset, _sb->GetSize()-offset);
				if(bytes2.size() == 0)
				{
					msreg_m.AddError("Ошибка тестирования. Ошибка конвертации файла PARAMS/DBNames");
					result = false;
				}
				else
				{
					sf = String((WCHART*)&bytes2[0], bytes2.size() / 2);
					for(i = 1; i <= sf.GetLength(); i++)
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
								if(_name.CompareIC("Fld") == 0) continue;
								if(_name.CompareIC("LineNo") == 0) continue;
								if(_name.CompareIC("Turnover") == 0) continue;
								if(_name.CompareIC("TurnoverDt") == 0) continue;
								if(_name.CompareIC("TurnoverCt") == 0) continue;
								if(_name.CompareIC("ByField") == 0) continue;
								if(_name.CompareIC("ByOwnerField") == 0) continue;
								if(_name.CompareIC("ByParentField") == 0) continue;
								if(_name.CompareIC("ByProperty") == 0) continue;
								if(_name.CompareIC("ByPropRecorder") == 0) continue;
								if(_name.CompareIC("ByResource") == 0) continue;
								if(_name.CompareIC("ByDim") == 0) continue;
								if(_name.CompareIC("ByDims") == 0) continue;
								if(_name.CompareIC("ByDimension") == 0) continue;
								if(_name.CompareIC("ByDimensions") == 0) continue;
								if(_name.CompareIC("ByDimRecorder") == 0) continue;
								if(_name.CompareIC("VT") == 0) is_slave = true;
								if(_name.CompareIC("ExtDim") == 0) is_slave = true;

								_guid = t->get_subnode(0)->get_value();
								_num = t->get_subnode(2)->get_value();

								if(_guid == "00000000-0000-0000-0000-000000000000") continue;

								_tabname = "_";
								_tabname += _name;
								_tabname += _num;
								l = _tabname.GetLength();

								bool table_found = false;
								for(i = 0; i < get_numtables(); i++)
								{
									if(is_slave)
									{
										sf = gettable(i)->getname();
										l2 = sf.GetLength();
										if(l2 > l) if(sf.SubString(l2 - l + 1, l).CompareIC(_tabname) == 0)
										{
											table_found = true;
											break;
										}
									}
									else if(gettable(i)->getname().CompareIC(_tabname) == 0)
									{
										table_found = true;
										break;
									}
								}


								if(!table_found)
								{
									msreg_m.AddMessage_("Отсутствует таблица", MessageState::Warning,
										"Имя таблицы", _tabname);
									result = false;
								}
							}

							delete rt;
						}
						else result = false;
					}
					else
					{
						msreg_m.AddError("Ошибка тестирования. Ошибка разбора файла PARAMS/DBNames. Первый символ не \"{\".");
						result = false;
					}
				}
			}
			else
			{
				msreg_m.AddError("Ошибка тестирования. Ошибка определения кодировки файла PARAMS/DBNames");
				result = false;
			}
		}

		delete _sb;

		break;

	}


	if(!hasDBNames)
	{
		msreg_m.AddError("Ошибка тестирования. В таблице PARAMS не найдена запись DBNames.");
		result = false;
	}

	return result;
}

//---------------------------------------------------------------------------
bool T_1CD::replaceTREF(String mapfile)
{
	vector<int32_t> map; // динамический массив соответствия номеров
	int32_t i,j,m;
	int32_t k, l;
	uint32_t ii, kk;
	char* rec;
	String str;
	TStringList* list;
	Table* t;
	Field* f;
	bool editsave;

	list = new TStringList;
	list->LoadFromFile(mapfile);

	m = 0;
	for(k = 0; k < list->Count(); k++)
	{
		str = (*list)[k];
		l = str.Pos("\t");
		if(!l) continue;
		j = str.SubString(l + 1, str.GetLength() - l).ToInt();
		if(m < j) m = j;
	}

	map.resize(m + 1);

	for(k = 0; k < list->Count(); k++)
	{
		str = (*list)[k];
		l = str.Pos("\t");
		if(!l) continue;
		i = str.SubString(1, l - 1).ToInt();
		j = str.SubString(l + 1, str.GetLength() - l).ToInt();
		map[j] = i;
	}

	delete list;

	for(i = 0; i < num_tables; i++)
	{
		t = gettable(i);
		for(j = 0; j < t->get_numfields(); j ++)
		{
			f = t->getfield(j);
			str = f->getname();
			if(str.GetLength() > 3)
			if(str.SubString(str.GetLength() - 3, 4).CompareIC("TREF") == 0)
			if(f->gettype() == type_fields::tf_binary)
			if(f->getlength() == 4)
			{
				msreg_m.Status(t->getname() + " : " + f->getname());
				k = f->getoffset();
				if(f->getnull_exists()) k++;
				rec = new char[t->get_recordlen()];
				editsave = t->edit;
				t->edit = true;
				for(kk = 0; kk < t->get_phys_numrecords(); kk++)
				{
					TableRecord *rec = t->getrecord(kk);
					if (rec->is_removed()) {
						continue;
					}
					ii = reverse_byte_order(*((uint32_t*)(rec + k))); // TODO: wat ???
					if(ii == 0) continue;
					ii = map[ii];
					*((int32_t*)(rec + k)) = reverse_byte_order(ii);
					t->write_data_record(kk, rec);
					delete rec;
				}
				t->edit = editsave;
			}
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
		msreg_m.AddMessage_("Попытка удаления объекта таблицы свободных блоков", MessageState::Warning,
			"Номер блока объекта", ob->block);
			return false;
	}

	if(ob->block == 2)
	{
		msreg_m.AddMessage_("Попытка удаления корневого объекта", MessageState::Warning,
			"Номер блока объекта", ob->block);
			return false;
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
	uint32_t i, k;
	int32_t j, numlosttables;
	char buf[8];
	v8object* v8obj;
	bool block_is_find;
	vector<uint32_t> losttables;
	char* b;

	numlosttables = 0;
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
		i = root_object->getlen();
		b = new char[i + numlosttables * 4];
		root_object->getdata(b, 0, i);

		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0)
		{
			root_80* root80 = (root_80*)b;
			for(j = 0, k = root80->numblocks; j < numlosttables; j++, k++) root80->blocks[k] = losttables[j];
			root80->numblocks += numlosttables;
		}
		else
		{
			root_81* root81 = (root_81*)b;
			for(j = 0, k = root81->numblocks; j < numlosttables; j++, k++) root81->blocks[k] = losttables[j];
			root81->numblocks += numlosttables;
		}
		root_object->setdata(b, i + numlosttables * 4);
		delete[] b;

	}

	msreg_m.AddMessage_("Поиск и восстановление потерянных таблиц завершены", MessageState::Succesfull,
	"Количество восстановленных таблиц", numlosttables);

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
				find_v8obj->savetofile(lost_objects.string() + "block" + i);
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

	int32_t i = ind->get_numrecords();
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

	int32_t version = version_presentation.ToIntDef(0);
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
		msreg_m.AddMessage_("Номер корневого блока файла DATA некорректный", MessageState::Error
			,"Таблица", tab->getname()
			,"Номер блока", block
		);
		return;
	}

	rootobj = (v8ob*)getblock_for_write(block, true);

	if(memcmp(rootobj->sig, SIG_OBJ, 8))
	{
		msreg_m.AddMessage_("Сигнатура корневого блока файла DATA некорректная.", MessageState::Error
			,"Таблица", tab->getname()
			,"Номер блока (dec)", block
			,"Номер блока (hex)", to_hex_string(block)
		);
		return;
	}

	l = rootobj->len;
	rl = tab->get_recordlen();
	if(l / rl * rl != l)
	{
		msreg_m.AddMessage_("Длина файла DATA не кратна длине одной записи.", MessageState::Error
			,"Таблица", tab->getname()
			,"Номер блока (dec)", block
			,"Номер блока (hex)", to_hex_string(block)
			,"Длина файла", l
			,"Длина записи", tab->get_recordlen()
		);
		return;
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
				msreg_m.AddMessage_("Некорректный номер блока таблицы размещения файла DATA. Создана новая страница размещения", MessageState::Warning
					,"Таблица", tab->getname()
					,"Индекс страницы", k
					,"Номер блока", a
				);
				a = length;
				ca = (objtab*)getblock_for_write(a, false);
			}
			else ca = (objtab*)getblock_for_write(a, true);

			n = ((uint64_t)l + 0xfff) >> 12;
			if(n > 1023) n = 1023;
			m = ca->numblocks;

			if(n != m)
			{
				msreg_m.AddMessage_("Некорректное число блоков на странице размещения файла DATA. Исправлено.", MessageState::Warning
					,"Таблица", tab->getname()
					,"Номер блока", a
					,"Индекс страницы", k
					,"Неверное количество блоков", m
					,"Верное количество блоков", n
				);
				ca->numblocks = n;
			}

			++k;
		}

		d = ca->blocks[j];
		ok = true;
		if(d < 5 || d >= length)
		{
			msreg_m.AddMessage_("Некорректный номер страницы данных файла DATA.", MessageState::Warning
				,"Таблица", tab->getname()
				,"Номер блока", a
				,"Индекс страницы размещения", k - 1
				,"Индекс блока на странице", j
				,"Неверный номер страницы", d
			);
			ok = false;
		}
		if(ok)
		{
			ok = test_block_by_template(d, rectt, i, rl, cl);
			if(!ok)
			{
				msreg_m.AddMessage_("Cтраница данных файла DATA не подходит по шаблону.", MessageState::Warning
				,"Таблица", tab->getname()
				,"Номер блока", d
				,"Индекс страницы размещения", k - 1
				,"Индекс блока на странице", j
				,"Индекс страницы в файле DATA", i
			);
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
				msreg_m.AddMessage_("Не удалось найти подходящую страницу данных файла DATA по шаблону.", MessageState::Error
					,"Таблица", tab->getname()
					,"Индекс страницы размещения", k - 1
					,"Индекс блока на странице", j
					,"Индекс страницы в файле DATA", i
				);
			}
			else if(bk.size() == 1)
			{
				d = bk[0];
				ca->blocks[j] = d;
				msreg_m.AddMessage_("Найдена подходящая страница данных файла DATA. Страница восстановлена", MessageState::Info
					,"Таблица", tab->getname()
					,"Номер блока", d
					,"Индекс страницы размещения", k - 1
					,"Индекс блока на странице", j
					,"Индекс страницы в файле DATA", i
				);

			}
			else
			{
				s = "";
				for(d = 0; d < bk.size(); ++d)
				{
					if(d > 0) s += ", ";
					s += to_hex_string(bk[d]);
				}
				msreg_m.AddMessage_("Найдено несколько подходящих страниц данных файла DATA.", MessageState::Hint
					,"Таблица", tab->getname()
					,"Список подходящих блоков", s
					,"Индекс страницы размещения", k - 1
					,"Индекс блока на странице", j
					,"Индекс страницы в файле DATA", i
				);
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

depot_ver T_1CD::get_depot_version(const TableRecord *record)
{
	String Ver = record->get_string("DEPOTVER");

	if (Ver.CompareIC("0300000000000000") == 0) {
		return depot_ver::Ver3;
	}
	if (Ver.CompareIC("0500000000000000") == 0) {
		return depot_ver::Ver5;
	}
	if (Ver.CompareIC("0600000000000000") == 0) {
		return depot_ver::Ver6;
	}
	if (Ver.CompareIC("0700000000000000") == 0) {
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
