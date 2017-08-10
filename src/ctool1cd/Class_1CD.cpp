//---------------------------------------------------------------------------

#include <vector>
#include <System.IOUtils.hpp>
#include <boost/filesystem.hpp>
#include <regex>

#include "UZLib.h"
#include "Class_1CD.h"
#include "Base64.h"
#include "Common.h"
#include "TempStream.h"
#include "ConfigStorage.h"
#include "Constants.h"

using namespace std;

extern Registrator msreg_g;

bool T_1CD::recoveryMode = false;

//********************************************************
// Функции

//---------------------------------------------------------------------------
tree* get_treeFromV8file(v8file* f)
{
	TBytesStream* sb;
	TEncoding *enc;
	TBytes bytes;
	int32_t offset;
	tree* rt;

	sb = new TBytesStream(bytes);
	f->SaveToStream(sb);

	enc = NULL;
	offset = TEncoding::GetBufferEncoding(sb->GetBytes(), enc);
	if(offset == 0)
	{
		msreg_g.AddError("Ошибка определения кодировки файла контейнера",
			"Файл", f->GetFullName());
		delete sb;
		return NULL;
	}
	bytes = TEncoding::Convert(enc, TEncoding::Unicode, sb->GetBytes(), offset, sb->GetSize() - offset);

	rt = parse_1Ctext(String((WCHART*)&bytes[0], bytes.GetLength() / 2), f->GetFullName());
	delete sb;
	return rt;
}


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
			"Индекс блока", tohex(block_number),
			"Всего блоков", tohex(length));
		return false;
	}

	memcpy(buf, MemBlock::getblock(fs, block_number), blocklen);
	return true;
}

//---------------------------------------------------------------------------
char*  T_1CD::getblock(uint32_t block_number)
{
	if(!fs) return NULL;
	if(block_number >= length)
	{
		msreg_m.AddError("Попытка чтения блока за пределами файла.",
			"Индекс блока", tohex(block_number),
			"Всего блоков", tohex(length));
		return NULL;
	}

	return MemBlock::getblock(fs, block_number);
}

//---------------------------------------------------------------------------
char*  T_1CD::getblock_for_write(uint32_t block_number, bool read)
{
	v8con* bc;


	if(!fs) return NULL;
	if(block_number > length)
	{
		msreg_m.AddError("Попытка получения блока за пределами файла базы.",
			"Индекс блока", tohex(block_number),
			"Всего блоков", tohex(length));
		return NULL;
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
		return NULL;
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

	is_infobase = false;
	is_depot    = false;
	Field::showGUIDasMS = false;

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
		free_blocks = NULL;
	}

	if(root_object)
	{
		delete root_object;
		root_object = NULL;
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
		tables = NULL;
	}
	num_tables = 0;

	// сначала закрываем кэшированные блоки (измененные блоки записывают себя в файл) ...
	MemBlock::delete_memblocks();

	// ... и только затем закрываем файл базы.
	if(fs)
	{
		delete fs;
		fs = NULL;
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
		fs = NULL;
		return;
	}

	v8con* cont = new v8con;
	fs->Read(cont, sizeof(v8con));

	if(memcmp(&(cont->sig), SIG_CON, 8) != 0)
	{
		msreg_m.AddError("Файл не является базой 1С (сигнатура не равна \"1CDBMSV8\")");
		delete fs;
		fs = NULL;
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
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
	}
	else if(ver == "8.2.0.0")
	{
		version = db_ver::ver8_2_0_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
	}
	else
#endif
		 if(ver == "8.2.14.0")
	{
		version = db_ver::ver8_2_14_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
	}
	else if(ver == "8.3.8.0")
	{
		version = db_ver::ver8_3_8_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
		pagesize = cont->pagesize;
	}
	else
	{
		msreg_m.AddError("Неподдерживаемая версия базы 1С",
			"Версия базы", ver);
		delete fs;
		fs = NULL;
		delete cont;
		return;
	}

	length = fs->GetSize() / pagesize;
	if((int64_t)length * pagesize != fs->GetSize())
	{
		msreg_m.AddError(String("Длина файла базы не кратна длине страницы (" + tohex(pagesize) + ")"),
			"Длина файла", tohex(fs->GetSize()));
		delete fs;
		fs = NULL;
		return;
	}

	MemBlock::pagesize = pagesize;
	MemBlock::maxcount = 0x40000000 / pagesize; // гигабайт
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
			Field::showGUIDasMS = true;
		}
	}
	else
	{
		is_infobase = true;
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
	return fs != NULL;
}

//---------------------------------------------------------------------------
db_ver T_1CD::getversion()
{
	return version;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config(String _filename)
{
	if(!cs_config) cs_config = new ConfigStorageTableConfig(get_files_config());
	if(!cs_config->getready()) return false;
	return cs_config->save_config(_filename);
}

//---------------------------------------------------------------------------
bool T_1CD::save_configsave(String _filename)
{
	if(!cs_configsave) cs_configsave = new ConfigStorageTableConfigSave(get_files_config(), get_files_configsave());
	if(!cs_configsave->getready()) return false;
	return cs_configsave->save_config(_filename);
}

//---------------------------------------------------------------------------
void T_1CD::find_supplier_configs()
{
	std::map<String,table_file*>::iterator p;

	for(p = get_files_configsave()->files().begin(); p != get_files_configsave()->files().end(); ++p)
	{
		if(p->first.GetLength() == 73) add_supplier_config(p->second);
	}
	for(p = get_files_config()->files().begin(); p != get_files_config()->files().end(); ++p)
	{
		if(p->first.GetLength() == 73) add_supplier_config(p->second);
	}
	supplier_configs_defined = true;
}

//---------------------------------------------------------------------------
void T_1CD::add_supplier_config(table_file* tf)
{
	container_file* f;
	TStream* s;
	int32_t i;
	v8catalog* cat = NULL;
	v8catalog* cat2;
	v8file* file;
	tree* tr = NULL;
	String filenamemeta;
	String nodetype;
	String _name; // имя конфигурация поставщика
	String _supplier; // синоним конфигурация поставщика
	String _version; // версия конфигурация поставщика
	SupplierConfig sc;

	f = new container_file(tf, tf->name);
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
		f = NULL;

		cat = new v8catalog(s, true);
		s = NULL;
		file = cat->GetFile("version");
		if(!file)
		{
			msreg_m.AddError("Не найден файл version в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name);
			delete cat;
			return;
		}

		tr = get_treeFromV8file(file);
		i = (*tr)[0][0][0].get_value().ToInt();
		delete tr;
		tr = NULL;

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
				msreg_m.AddError("Файл metadata неявляется каталогом в конфигурации поставщика",
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

		tr = get_treeFromV8file(file);
		filenamemeta = (*tr)[0][1].get_value();
		delete tr;
		tr = NULL;

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

		tr = get_treeFromV8file(file);
		int32_t numnode = (*tr)[0][2].get_value().ToInt();
		for(i = 0; i < numnode; i++)
		{
			tree& node = (*tr)[0][3 + i];
			nodetype = node[0].get_value();
			if(nodetype.CompareIC("9cd510cd-abfc-11d4-9434-004095e12fc7") == 0) // узел "Общие"
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
		tr = NULL;

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


		sc.file = tf;
		sc.name = _name;
		sc.supplier = _supplier;
		sc.version = _version;
		supplier_configs.push_back(sc);

		delete cat;
		cat = NULL;
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
bool T_1CD::save_supplier_configs(uint32_t numcon, const String& _filename)
{
	TFileStream* _fs;
	container_file* f;
	table_file* tf;

	if(numcon >= supplier_configs.size()) return false;
	tf = supplier_configs[numcon].file;
	f = new container_file(tf, tf->name);
	if(!f->open())
	{
		delete f;
		return false;
	}

	try
	{
		_fs = new TFileStream(_filename, fmCreate);
	}
	catch(...)
	{
		msreg_m.AddError("Ошибка открытия файла конфигурации поставщика",
			"Имя файла", _filename);
		delete f;
		return false;
	}

	try
	{
		ZInflateStream(f->stream, _fs);
	}
	catch(...)
	{
		msreg_m.AddError("Ошибка распаковки файла конфигурации поставщика",
			"Имя файла", _filename);
		delete f;
		delete _fs;
		return false;
	}

	delete _fs;
	delete f;
	return true;
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

#ifndef PublicRelease
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
			if(!block_is_find) msreg_m.AddMessage_("Найден потерянный объект", MessageState::Info, "Номер блока", tohex(i));
		}
	}
	msreg_m.AddMessage("Поиск потерянных объектов завершен", MessageState::Succesfull);
}
#endif //#ifdef PublicRelease

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
	char* rec;
	char* orec;
	Field* f_name;
	Field* f_data_size;
	Field* f_binary_data;
	String path;
	String slen;
	TStream* str;
	bool result;
	bool res;


	rec = new char[t->get_recordlen()];
	t->getrecord(nrec, rec);
	if(*rec)
	{
		delete[] rec;
		return true;
	}

	f_name = t->getfield(0);
	f_data_size = t->getfield(4);
	f_binary_data = t->getfield(5);

	path = t->getname() + "/" + f_name->get_presentation(rec);

	orec = rec + f_binary_data->getoffset();
	if(*(uint32_t*)(orec + 4) > 10 * 1024 * 1024) str = new TTempStream;
	else str = new TMemoryStream();
	t->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

	result = true;
	slen = f_data_size->get_presentation(rec, true);
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

	res = recursive_test_stream_format(str, path, f_name->get_presentation(rec).GetLength() > 72); // вторично упакованы могут быть только конфигурации поставщика (файлы с длиной имени более 72 символов)
	result = result && res;

	delete[] rec;
	delete str;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format2(Table* t, uint32_t nrec)
{
	char* rec;
	char* orec;
	Field* f_sd;
	String path;
	TMemoryStream* str;
	bool result;

	rec = new char[t->get_recordlen()];
	t->getrecord(nrec, rec);
	if(*rec)
	{
		delete[] rec;
		return true;
	}

	f_sd = t->getfield(0);

	path = t->getname();

	orec = rec + f_sd->getoffset();
	str = new TMemoryStream();
	t->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

	result = recursive_test_stream_format(str, path);

	delete[] rec;
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
	TBytes bytes1;
	TBytes bytes2;
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
		cat = NULL;

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

		enc = NULL;
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
				if(bytes2.GetLength() == 0)
				{
					msreg_m.AddError("Ошибка тестирования. Ошибка конвертации",
						"Путь", path);
					result = false;
				}
				else
				{
					sf = String((WCHART*)&bytes2[0], bytes2.GetLength() / 2);
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
			c = NULL;
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
#ifndef PublicRelease
bool T_1CD::create_table(String path)
{
	TFileStream* f;
	bool fopen;
	String dir;
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

	if(!DirectoryExists(path))
	{
		msreg_m.AddMessage_("Директория импорта таблицы не найдена", MessageState::Warning,
			"Директория", path);
		return false;
	}
	dir = path + "\\";

	try
	{
		f = new TFileStream(dir + "root", fmOpenRead);
	}
	catch(...)
	{
		msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы root", MessageState::Warning,
			"Файл", dir + "root");
		return false;
	}
	root = new export_import_table_root;
	f->Read(root, sizeof(export_import_table_root));
	delete f;


	try
	{
		f = new TFileStream(dir + "descr", fmOpenRead);
	}
	catch(...)
	{
		msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы descr", MessageState::Warning,
			"Файл", dir + "descr");
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

	t = parse_1Ctext(str, dir + "descr");
	str = (*t)[0][0].get_value();

	for(j = 0; j < num_tables; j++) if(tables[j]->getname().CompareIC(str) == 0)
	{
		delete_table(tables[j]);
	}

	file_data = NULL;
	file_blob = NULL;
	file_index = NULL;

	if(root->has_data)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "data", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы data", MessageState::Warning,
				"Файл", dir + "data");
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
		try
		{
			f = new TFileStream(dir + "blob", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы blob", MessageState::Warning,
				"Файл", dir + "blob");
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
		try
		{
			f = new TFileStream(dir + "index", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы index", MessageState::Warning,
				"Файл", dir + "index");
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
			f = new TFileStream(dir + "descr", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			msreg_m.AddMessage_("Ошибка открытия файла импорта таблицы descr", MessageState::Warning,
				"Файл", dir + "descr");
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
					"Файл", dir + "descr");
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
		"Путь", dir);

	delete root;
	return true;
}
#endif //#ifdef PublicRelease

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
	TBytes bytes1;
	TBytes bytes2;
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

		table_params->getrecord(k, rec);
		if(*rec) continue;

		if(f_name->get_presentation(rec).CompareIC("DBNames") != 0) continue;

		hasDBNames = true;

		orec = rec + f_binary_data->getoffset();
		str = new TMemoryStream();
		table_params->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

		slen = f_data_size->get_presentation(rec, true);
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

		enc = NULL;
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
				if(bytes2.GetLength() == 0)
				{
					msreg_m.AddError("Ошибка тестирования. Ошибка конвертации файла PARAMS/DBNames");
					result = false;
				}
				else
				{
					sf = String((WCHART*)&bytes2[0], bytes2.GetLength() / 2);
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

	delete[] rec;

	return result;
}

#ifndef PublicRelease
//---------------------------------------------------------------------------
bool T_1CD::replaceTREF(String mapfile)
{
	DynamicArray<int32_t> map; // динамический массив соответствия номеров
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

	map.set_length(m + 1);

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
					t->getrecord(kk, rec);
					if(*rec) continue;
					ii = reverse_byte_order(*((uint32_t*)(rec + k)));
					if(ii == 0) continue;
					ii = map[ii];
					*((int32_t*)(rec + k)) = reverse_byte_order(ii);
					t->write_data_record(kk, rec);
				}
				t->edit = editsave;

				delete[] rec;
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
		tab->descr_table = NULL;
		tab->file_data = NULL;
		tab->file_blob = NULL;
		tab->file_index = NULL;


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
	DynamicArray<uint32_t> losttables;
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
							if(losttables.get_length() <= numlosttables) losttables.set_length(losttables.get_length() + 1024);
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
void T_1CD::find_and_save_lost_objects()
{
	uint32_t i;
	char buf[8];
	v8object* v8obj;
	bool block_is_find;
	bool dir_created;
	String path;

	dir_created = false;
	path = filename.SubString(1, filename.LastDelimiter("\\"));
	path += "LostObjects\\";

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
				if(!dir_created)
				{
					CreateDir(path);
					dir_created = true;
				}
				v8obj = new v8object(this, i);
				v8obj->savetofile(path + "block" + i);
				delete v8obj;
			}
		}
	}
	msreg_m.AddMessage("Поиск и сохранение потерянных объектов завершен", MessageState::Succesfull);

}

#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
// Если не удалось получить версию, возвращается 0, иначе возвращается положительное число
int32_t T_1CD::get_ver_depot_config(int32_t ver) // Получение номера версии конфигурации (0 - последняя, -1 - предпоследняя и т.д.)
{
	char* rec;
	Index* ind;
	Field* fld;
	uint32_t i;
	int32_t v;
	String s;

	if(!is_open()) return 0;

	if(!is_depot)
	{
		msreg_m.AddError("База не является хранилищем конфигурации.");
		return 0;
	}

	if(ver > 0) return ver;

	// Определяем номер последней версии конфигурации
	if(!table_versions)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица VERSIONS.");
		return 0;
	}

	fld = get_field(table_versions, "VERNUM");
	if(!fld) return 0;

	ind = get_index(table_versions, "PK");
	if(!ind) return 0;

	i = ind->get_numrecords();
	if(i <= (uint32_t)(-ver))
	{
		msreg_m.AddMessage_("Запрошенной версии конфигурации не существует", MessageState::Error,
			"Всего версий в хранилище", i,
			"Запрошенный номер версии", ver);
		return 0;
	}
	i = ind->get_numrec(i + ver - 1);

	rec = new char[table_versions->get_recordlen()];
	table_versions->getrecord(i, rec);
	s = fld->get_presentation(rec, true);
	delete[] rec;
	v = s.ToIntDef(0);
	if(!v)
	{
		msreg_m.AddMessage_("Не удалось получить реальный номер версии запрошенной конфигурации.", MessageState::Error,
			"Запрошенный номер версии", ver);
		return 0;
	}

	return v;
}

Field* T_1CD::get_field(Table* tab, String fieldname)
{
	int32_t j;
	Field* fld;
	String s;

	for(j = 0; j < tab->num_fields; j++)
	{
		fld = tab->fields[j];
		if(fld->getname().CompareIC(fieldname) == 0) return fld;
	}

	s ="В таблице ";
	s += tab->name;
	s += " не найдено поле ";
	s += fieldname;
	s += ".";
	msreg_m.AddError(s);

	return NULL;
}

Index* T_1CD::get_index(Table* tab, String indexname)
{
	int32_t j;
	Index* ind;
	String s;

	for(j = 0; j < tab->num_indexes; j++)
	{
		ind = tab->indexes[j];
		if(ind->getname().CompareIC(indexname) == 0) return ind;
	}

	s ="В таблице ";
	s += tab->name;
	s += " не найден индекс ";
	s += indexname;
	s += ".";
	msreg_m.AddError(s);

	return NULL;
}

//---------------------------------------------------------------------------
// Сохранение конфигурации в файл из хранилища конфигураций
// ver - номер версии сохраняемой конфигурации
// ver > 0 - используется переданный номер версии
// ver <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_depot_config(const String& _filename, int32_t ver)
{
	char* rec;
	char* frec;
	Field* fldd_depotver;
	Field* fldd_rootobjid;

	Field* fldv_vernum;
	Field* fldv_cversion;
	Field* fldv_snapshotcrc;
	Field* fldv_snapshotmaker;

	Field* fldh_objid;
	Field* fldh_vernum;
	Field* fldh_objverid;
	Field* fldh_removed;
	Field* fldh_datapacked;
	Field* fldh_objdata;
	Field* fldh_datahash;
	Index* indh;
	char* rech1;
	char* rech2;

	char rootobj[16];
	char curobj[16];
	uint32_t ih, nh;

	Field* flde_objid;
	Field* flde_vernum;
	Field* flde_extname;
	Field* flde_extverid;
	Field* flde_datapacked;
	Field* flde_extdata;
	Field* flde_datahash;
	Index* inde;
	char* rece;
	DynamicArray<char*> reces;
	DynamicArray<String> extnames;
	int32_t nreces;
	uint32_t ie, ne;

	bool ok;
	bool lastremoved;
	bool removed;
	bool datapacked;
	bool deletesobj;
	char emptyimage[8];
	uint32_t i, k;
	int32_t v, j, res, lastver, n;
	String sn;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TStream* in;
	TStream* out;
	TStream* st;
	std::vector<_packdata> packdates;
	TSearchRec srec;
	_packdata pd;
	_packdata* pdr;
	int64_t packlen;
	v8catalog* cat;
	v8catalog* cath;
	bool oldformat;
	v8file* f;
	tree* t;
	tree* tc;
	tree* tv; // корень дерева файла versions
	tree* tvc; // тек. элемент дерева файла versions
	tree* tr; // корень дерева файла root
	tree* trc; // тек. элемент дерева файла root
	tree* tcountv; // узел, содержащий счетчик в файле versions
	tree* tcountr; // узел, содержащий счетчик в файле root

	union
	{
		GUID guid;
		unsigned char uid[16];
	};

	union
	{
		char cv_b[2];
		unsigned short cv_s;
	};


	if(!is_open()) return false;

	if(!is_depot)
	{
		msreg_m.AddError("База не является хранилищем конфигурации.");
		return false;
	}

	// Получаем версию хранилища
	if(!table_depot)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица DEPOT.");
		return false;
	}

	fldd_depotver = get_field(table_depot, "DEPOTVER");
	if(!fldd_depotver) return false;
	fldd_rootobjid = get_field(table_depot, "ROOTOBJID");
	if(!fldd_rootobjid) return false;

	rec = new char[table_depot->get_recordlen()];
	ok = false;
	for(i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		table_depot->getrecord(i, rec);
		if(!*rec)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		msreg_m.AddError("Не удалось прочитать запись в таблице DEPOT.");
		delete[] rec;
		return false;
	}

	{
	String s = fldd_depotver->get_presentation(rec, true);

	if(s.CompareIC("0300000000000000") == 0) depotVer = depot_ver::Ver3;
	else if(s.CompareIC("0500000000000000") == 0) depotVer = depot_ver::Ver5;
	else if(s.CompareIC("0600000000000000") == 0) depotVer = depot_ver::Ver6;
	else if(s.CompareIC("0700000000000000") == 0) depotVer = depot_ver::Ver7;
	else
	{
		msreg_m.AddMessage_("Неизвестная версия хранилища", MessageState::Error,
			"Версия хранилища", s);
		delete[] rec;
		return false;
	}
	}

	memcpy(rootobj, rec + fldd_rootobjid->offset, 16);
	delete[] rec;

	// "Нормализуем" версию конфигурации
	ver = get_ver_depot_config(ver);

	// Ищем строку с номером версии конфигурации
	if(!table_versions)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица VERSIONS.");
		return false;
	}

	fldv_vernum = get_field(table_versions, "VERNUM");
	if(!fldv_vernum) return false;
	if(depotVer >= depot_ver::Ver5)
	{
		fldv_cversion = get_field(table_versions, "CVERSION");
		if(!fldv_cversion) return false;
	}
	fldv_snapshotcrc = get_field(table_versions, "SNAPSHOTCRC");
	if(!fldv_snapshotcrc) return false;
	fldv_snapshotmaker = get_field(table_versions, "SNAPSHOTMAKER");
	if(!fldv_snapshotmaker) return false;

	rec = new char[table_versions->get_recordlen()];
	ok = false;
	for(i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		table_versions->getrecord(i, rec);
		if(*rec) continue;
		String s = fldv_vernum->get_presentation(rec, true);
		v = s.ToIntDef(0);
		if(v == ver)
		{
			ok = true;
			break;
		}
	}

	if(!ok)
	{
		msreg_m.AddMessage_("В хранилище не найдена версия конфигурации", MessageState::Error,
			"Требуемая версия", ver);
		delete[] rec;
		return false;
	}

	boost::filesystem::path filepath = boost::filesystem::path(static_cast<std::string>(_filename));

	// Определяем версию структуры конфигурации (для файла version)
	if(depotVer >= depot_ver::Ver5)
	{
		frec = rec + fldv_cversion->offset;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMajor = cv_s;
		frec += 2;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMinor = cv_s;
	}
	else
	{
		configVerMinor = 0;
		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0) configVerMajor = 6;
		else if(version == db_ver::ver8_1_0_0) configVerMajor = 106;
		else configVerMajor = 216;
	}

	delete[] rec;

	// Инициализируем таблицы HISTORY и EXTERNALS
	if(!table_history)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица HISTORY.");
		return false;
	}

	if(!table_externals)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица EXTERNALS.");
		return false;
	}

	fldh_objid = get_field(table_history, "OBJID");
	if(!fldh_objid) return false;
	fldh_vernum = get_field(table_history, "VERNUM");
	if(!fldh_vernum) return false;
	fldh_objverid = get_field(table_history, "OBJVERID");
	if(!fldh_objverid) return false;
	fldh_removed = get_field(table_history, "REMOVED");
	if(!fldh_removed) return false;
	fldh_datapacked = get_field(table_history, "DATAPACKED");
	if(!fldh_datapacked) return false;
	fldh_objdata = get_field(table_history, "OBJDATA");
	if(!fldh_objdata) return false;

	flde_objid = get_field(table_externals, "OBJID");
	if(!flde_objid) return false;
	flde_vernum = get_field(table_externals, "VERNUM");
	if(!flde_vernum) return false;
	flde_extname = get_field(table_externals, "EXTNAME");
	if(!flde_extname) return false;
	flde_extverid = get_field(table_externals, "EXTVERID");
	if(!flde_extverid) return false;
	flde_datapacked = get_field(table_externals, "DATAPACKED");
	if(!flde_datapacked) return false;
	flde_extdata = get_field(table_externals, "EXTDATA");
	if(!flde_extdata) return false;

	boost::filesystem::path root_path(static_cast<std::string>(filename)); // путь к 1cd
	boost::filesystem::path objects_path;

	if(depotVer >= depot_ver::Ver6)
	{
		fldh_datahash = get_field(table_history, "DATAHASH");
		if(!fldh_datahash) return false;
		flde_datahash = get_field(table_externals, "DATAHASH");
		if(!flde_datahash) return false;

		boost::filesystem::path subpath = root_path.parent_path() / "data" / "pack";
		std::regex pack_mask("pack-.*\\.ind");
		boost::filesystem::directory_iterator dit(subpath), dend;
		for (; dit != dend; dit++)
		{
			boost::filesystem::path current_path = dit->path();
			if (!std::regex_match(current_path.filename().string(), pack_mask)) {
				continue;
			}
				try
				{
					in = new TFileStream(current_path.string(), fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
					msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
						"Файл", current_path.string());
					return false;
				}
				in->Seek(8, soFromBeginning);
				in->Read(&i, 4);
				pd.datahashes = new _datahash[i];
				in->Read(pd.datahashes, i * sizeof(_datahash));
				delete in;
				pd.count = i;

				boost::filesystem::path pack_item = current_path;
				pack_item.replace_extension("pck");
				try
				{
					pd.pack = new TFileStream(pack_item.string(), fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
				    delete pd.pack;
				    msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
						"Файл", pack_item.string());
					return false;
				}
				packdates.push_back(pd);
		}

		objects_path = root_path / "data" / "objects";
	}
	else
	{
		fldh_datahash = NULL;
		flde_datahash = NULL;
	}

	indh = get_index(table_history, "PK");
	if(!indh) return 0;
	inde = get_index(table_externals, "PK");
	if(!inde) return 0;

	rech1 = new char[table_history->get_recordlen()];
	rech2 = new char[table_history->get_recordlen()];
	rece = new char[table_externals->get_recordlen()];
	memset(rece, 0, table_externals->get_recordlen());
	nreces = 0;
	reces.SetLength(0);

	nh = indh->get_numrecords();
	ne = inde->get_numrecords();
	memset(curobj, 0, 16);

	if (boost::filesystem::exists(filepath)) {
		boost::filesystem::remove(filepath);
	}
	cat = new v8catalog(filepath.string(), false);

	// root, versions

	std::map<String,String> vermap; // контейнер для сортировки versions
	std::map<String,String> rootmap; // контейнер для сортировки root
	std::map<String,TStream*> extmap; // контейнер для сортировки файлов в корне
	std::map<String,TStream*> metamap; // контейнер для сортировки файлов в metadata

	tv = new tree("", nd_list, NULL); // корень дерева файла versions
	tvc = new tree("", nd_list, tv); // тек. элемент дерева файла versions
	tr = new tree("", nd_list, NULL); // корень дерева файла root
	trc = new tree("", nd_list, tr); // тек. элемент дерева файла root

	tvc->add_child("1", nd_number);
	tcountv = tvc->add_child("0", nd_number); // узел, содержащий счетчик в файле versions

	CreateGUID(guid);
	vermap[""] = GUIDasMS(uid);

	String sversion;
	{// Создаем и записываем файл version
	String s;
	t = new tree("", nd_list, NULL);
	tc = new tree("", nd_list, t);
	tc = new tree("", nd_list, tc);
	s = configVerMajor;
	tc->add_child(s, nd_number);
	s = configVerMinor;
	tc->add_child(s, nd_number);
	sversion = outtext(t);
	delete t;
	}

	in = new TMemoryStream;
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().GetLength());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(sversion);
	}
	out = new TMemoryStream;
	in->Seek(0, soFromBeginning);
	ZDeflateStream(in, out);
	delete in;
	extmap["version"] = out;
	CreateGUID(guid);
	vermap["version"] = GUIDasMS(uid);

	if(configVerMajor < 100)
	{
		trc->add_child("1", nd_number);
		trc->add_child(GUIDasMS((unsigned char*)rootobj), nd_guid);
		tcountr = trc->add_child("0", nd_number); // узел, содержащий счетчик в файле root
		oldformat = true;
	}
	else
	{
		trc->add_child("2", nd_number);
		trc->add_child(GUIDasMS((unsigned char*)rootobj), nd_guid);
		tcountr = NULL;
		oldformat = false;
	}

	lastver = -1;
	lastremoved = true;
	memset(emptyimage, 0, 8);

	in = new TMemoryStream;
	for(ih = 0, ie = 0; ih <= nh; ih++)
	{
		if(ih < nh)
		{
			i = indh->get_numrec(ih);
			table_history->getrecord(i, rech2);
		}

		if(memcmp(curobj, rech2 + fldh_objid->offset, 16) != 0 || ih == nh)
		{ // это новый объект или конец таблицы
			if(!lastremoved)
			{
				ok = false;
				deletesobj = false;
				packlen = 0;
				rec = rech1 + fldh_objdata->offset + 1;
				if(*(rech1 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
				{
					out = new TTempStream;
					if(oldformat)
					{
						table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
						in->Seek(0, soFromBeginning);
						ZInflateStream(in, out);
					}
					else table_history->readBlob(out, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
					out->Close();
					ok = true;
				}
				else if(depotVer >= depot_ver::Ver6)
				{
					rec = rech1 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0);
					for(i = 0; i < packdates.size(); i++)
					{
						pdr = &packdates[i];
						for(k = 0; k < pdr->count; k++) if(memcmp(rec, pdr->datahashes[k].datahash, 20) == 0)
						{
							st = pdr->pack;
							st->Seek(pdr->datahashes[k].offset, soBeginning);
							st->Read(&packlen, 8);
							out = new TTempStream;
							out->CopyFrom(st, packlen);
							ok = true;
							out->Close();
							break;
						}
						if(ok) break;
					}

					if(!ok)
					{
						String ss = fldh_datahash->get_presentation(rech1, true);
						boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
						if(boost::filesystem::exists(current_object_path))
						{
							try
							{
								out = new TFileStream(current_object_path.string(), fmOpenRead | fmShareDenyNone);
								ok = true;
							}
							catch(...)
							{
								msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
									"Файл", current_object_path.string(),
									"Таблица", "HISTORY",
									"Объект", fldh_objid->get_presentation(rech1, false, L'.', true),
									"Версия", fldh_vernum->get_presentation(rech1, false));
							}
						}
						else
						{
							msreg_m.AddMessage_("Не найден файл", MessageState::Error,
								"Файл", current_object_path.string(),
								"Таблица", "HISTORY",
								"Объект", fldh_objid->get_presentation(rech1, false, L'.', true),
								"Версия", fldh_vernum->get_presentation(rech1, false));
						}
					}
				}
				String s = fldh_objid->get_presentation(rech1, false, L'.', true);
				if(!ok)
				{
					msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
						"Таблица", "HISTORY",
						"Объект", s,
						"Версия", fldh_vernum->get_presentation(rech1, false));
				}
				else
				{
					if(oldformat)
					{
						rootmap[s] = GUIDasMS((unsigned char*)rech1 + fldh_objverid->offset);
						metamap[s] = out;
					}
					else
					{
						vermap[s] = GUIDasMS((unsigned char*)rech1 + fldh_objverid->offset);
						extmap[s] = out;
					}


					// Вот тут идем по EXTERNALS
					while(true)
					{
						if(ie > ne) break;
						res = memcmp(rece + flde_objid->offset, curobj, 16);
						if(res > 0) break;
						if(!res)
						{
							s = flde_vernum->get_presentation(rece, false);
							v = s.ToIntDef(MaxInt);
							s = flde_extname->get_presentation(rece);
							if(v <= ver) if(*(rece + flde_datapacked->offset))
							{
								for(j = 0; j < nreces; j++) if(s.CompareIC(flde_extname->get_presentation(reces[j])) == 0) break;
								if(j == reces.GetLength()){
									reces.SetLength(reces.GetLength() + 1);
									reces[j] = new char[table_externals->get_recordlen()];
								}
								if(j == nreces) nreces++;
								memcpy(reces[j], rece, table_externals->get_recordlen());
							}
							if(v == lastver)
							{
								extnames.SetLength(extnames.GetLength() + 1);
								extnames[extnames.GetLength() - 1] = s;
							}
						}
						if(ie == ne)
						{
							ie++;
							break;
						}
						i = inde->get_numrec(ie++);
						table_externals->getrecord(i, rece);
					}
					for(j = 0; j < nreces; j++)
					{
						rec = reces[j];
						sn = flde_extname->get_presentation(rec);
						ok = false;
						for(n = 0; n < extnames.GetLength(); n++ ) if(sn.CompareIC(extnames[n]) == 0)
						{
							ok = true;
							break;
						}
						if(!ok) continue;

						ok = false;
						packlen = 0;
						deletesobj = false;
						frec = rec + flde_extdata->offset;
						if(memcmp(emptyimage, frec, 8))
						{
							out = new TTempStream;
							table_externals->readBlob(out, *(uint32_t*)frec, *(uint32_t*)(frec + 4));
							out->Close();
							ok = true;
						}
						else if(depotVer >= depot_ver::Ver6)
						{
							frec = rec + flde_datahash->offset + (flde_datahash->null_exists ? 1 : 0);
							for(i = 0; i < packdates.size(); i++)
							{
								pdr = &packdates[i];
								for(k = 0; k < pdr->count; k++) if(memcmp(frec, pdr->datahashes[k].datahash, 20) == 0)
								{
									out = new TTempStream;
									st = pdr->pack;
									st->Seek(pdr->datahashes[k].offset, soBeginning);
									st->Read(&packlen, 8);
									out->CopyFrom(st, packlen);
									ok = true;
									out->Close();
									break;
								}
								if(ok) break;
							}

							if(!ok)
							{
								String ss = flde_datahash->get_presentation(rec, true);
								boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
								if (boost::filesystem::exists(current_object_path))
								{
									try
									{
										out = new TFileStream(current_object_path.string(), fmOpenRead | fmShareDenyNone);
										ok = true;
									}
									catch(...)
									{
										msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
											"Файл", current_object_path.string(),
											"Таблица", "EXTERNALS",
											"Объект", flde_extname->get_presentation(rec),
											"Версия", flde_vernum->get_presentation(rec));
									}
								}
								else
								{
									msreg_m.AddMessage_("Не найден файл", MessageState::Error,
										"Файл", current_object_path.string(),
										"Таблица", "EXTERNALS",
										"Объект", flde_extname->get_presentation(rec),
										"Версия", flde_vernum->get_presentation(rec));
								}
							}
						}
						if(!ok)
						{
							msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
								"Таблица", "EXTERNALS",
								"Объект", sn,
								"Версия", flde_vernum->get_presentation(rec));
						}
						else
						{
							vermap[sn] = GUIDasMS((unsigned char*)rec + flde_extverid->offset);
							extmap[sn] = out;
						}

					}
					nreces = 0;
					extnames.SetLength(0);
				}
			}

			memcpy(curobj, rech2 + fldh_objid->offset, 16);
			lastremoved = true;
		}

		if(ih < nh)
		{
			String s = fldh_vernum->get_presentation(rech2, false);
			v = s.ToIntDef(MaxInt);
			if(v <= ver)
			{
				removed = *(rech2 + fldh_removed->offset);
				if(removed)
				{
					lastremoved = true;
				}
				else
				{
					datapacked = false;
					if(*(rech2 + fldh_datapacked->offset)) if(*(rech2 + fldh_datapacked->offset + 1)) datapacked = true;
					if(datapacked)
					{
						memcpy(rech1, rech2, table_history->get_recordlen());
						lastremoved = false;
						lastver = v;
					}
				}
			}
		}
	}

	delete[] rech1;
	delete[] rech2;
	delete[] rece;
	for(j = 0; j < nreces; j++) delete[] reces[j];


	// Завершаем формирование списков версий
	std::map<String,String>::iterator pmap;

	CreateGUID(guid);
	vermap["versions"] = GUIDasMS(uid);
	if(oldformat)
	{
		tcountv->set_value(vermap.size(), nd_number);
		tcountr->set_value(rootmap.size(), nd_number);
	}
	else
	{
		CreateGUID(guid);
		vermap["root"] = GUIDasMS(uid);
		tcountv->set_value(vermap.size(), nd_number);
	}

	// Запись root
	for(pmap = rootmap.begin(); pmap != rootmap.end(); ++pmap)
	{
		trc->add_child(pmap->first, nd_string);
		trc->add_child(pmap->second, nd_guid);
	}
	String tree_text = outtext(tr);
	delete tr;
	in->SetSize(0);
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().GetLength());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(tree_text);
	}
	in->Seek(0, soFromBeginning);
	out = new TTempStream;
	if(oldformat)
	{
		out->CopyFrom(in, 0);
		metamap["root"] = out;
	}
	else
	{
		ZDeflateStream(in, out);
		extmap["root"] = out;
	}

	// Запись versions

	for(pmap = vermap.begin(); pmap != vermap.end(); ++pmap)
	{
		tvc->add_child(pmap->first, nd_string);
		tvc->add_child(pmap->second, nd_guid);
	}

	String tv_text = outtext(tv);
	delete tv;
	in->SetSize(0);
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().GetLength());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(tv_text);
	}
	out = new TTempStream;
	in->Seek(0, soFromBeginning);
	ZDeflateStream(in, out);
	extmap["versions"] = out;

	std::map<String,TStream*>::iterator psmap;
	if(oldformat)
	{
		cath = cat->CreateCatalog("metadata", true);
		for(psmap = metamap.begin(); psmap != metamap.end(); ++psmap)
		{
			f = cath->createFile(psmap->first);
			f->WriteAndClose(psmap->second);
			delete psmap->second;
		}
	}
	for(psmap = extmap.begin(); psmap != extmap.end(); ++psmap)
	{
		f = cat->createFile(psmap->first);
		f->WriteAndClose(psmap->second);
		delete psmap->second;
	}

	delete in;

	for(i = 0; i < packdates.size(); i++)
	{
		delete packdates[i].pack;
		delete[] packdates[i].datahashes;
	}

	delete cat;
	return true;
}

//---------------------------------------------------------------------------
// Сохранение файлов конфигурации в каталог из хранилища конфигураций
// ver_begin - начальный номер диапазона версий сохраняемых файлов конфигурации
// ver_end - конечный номер диапазона версий сохраняемых файлов конфигурации
// ver_begin > 0, ver_end > 0 - используется переданный номер версии
// ver_begin <= 0, ver_end <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_part_depot_config(const String& _filename, int32_t ver_begin, int32_t ver_end)
{
	char* rec;
	char* frec;
	Field* fldd_depotver;

	Field* fldv_vernum;
	Field* fldv_cversion;
	Field* fldv_snapshotcrc;
	Field* fldv_snapshotmaker;

	Field* fldh_objid;
	Field* fldh_vernum;
	Field* fldh_objverid;
	Field* fldh_removed;
	Field* fldh_datapacked;
	Field* fldh_objdata;
	Field* fldh_datahash;
	Index* indh;
	char* rech; // текущая запись HISTORY
	char* rech1; // запись с версией < ver_begin
	bool hasrech1;
	char* rech2; // запись с версией <= ver_end
	bool hasrech2;

	//char rootobj[16];
	char curobj[16];
	uint32_t ih, nh;

	Field* flde_objid;
	Field* flde_vernum;
	Field* flde_extname;
	Field* flde_extverid;
	Field* flde_datapacked;
	Field* flde_extdata;
	Field* flde_datahash;
	Index* inde;
	char* rece;
	uint32_t ie, ne, je;

	bool ok;
	bool removed;
	bool datapacked;
	bool deletesobj;
	bool iscatalog;
	bool changed;
	bool inreaded;
	bool hasext;
	char emptyimage[8];
	char verid[16];
	uint32_t i, k;
	int32_t v, res, lastver, n;
	String s, ss, sp, sn, se;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TMemoryStream* in;
	TMemoryStream* out;
	TStream* sobj;
	TStreamWriter* sw;
	TMemoryStream* sd;
	bool hasdeleted;
	std::vector<_packdata> packdates;
	TSearchRec srec;
	_packdata pd;
	_packdata* pdr;
	int64_t packlen;
	v8catalog* cat;
	String cath;
	TFileStream* f;
	String __filename;

	union
	{
		GUID guid;
		unsigned char uid[16];
	};

	union
	{
		char cv_b[2];
		unsigned short cv_s;
	};


	if(!is_open()) return false;

	if(!is_depot)
	{
		msreg_m.AddError("База не является хранилищем конфигурации.");
		return false;
	}

	// Получаем версию хранилища
	if(!table_depot)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица DEPOT.");
		return false;
	}

	fldd_depotver = get_field(table_depot, "DEPOTVER");
	if(!fldd_depotver) return false;

	rec = new char[table_depot->get_recordlen()];
	ok = false;
	for(i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		table_depot->getrecord(i, rec);
		if(!*rec)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		msreg_m.AddError("Не удалось прочитать запись в таблице DEPOT.");
		delete[] rec;
		return false;
	}

	s = fldd_depotver->get_presentation(rec, true);

	if(s.CompareIC("0300000000000000") == 0) depotVer = depot_ver::Ver3;
	else if(s.CompareIC("0500000000000000") == 0) depotVer = depot_ver::Ver5;
	else if(s.CompareIC("0600000000000000") == 0) depotVer = depot_ver::Ver6;
	else
	{
		msreg_m.AddMessage_("Неизвестная версия хранилища", MessageState::Error,
			"Версия хранилища", s);
		delete[] rec;
		return false;
	}

	delete[] rec;

	// "Нормализуем" версию конфигурации
	ver_begin = get_ver_depot_config(ver_begin);
	ver_end = get_ver_depot_config(ver_end);
	if(ver_end < ver_begin)
	{
		v = ver_begin;
		ver_begin = ver_end;
		ver_end = v;
	}

	// Ищем строку с номером версии конфигурации
	if(!table_versions)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица VERSIONS.");
		return false;
	}

	fldv_vernum = get_field(table_versions, "VERNUM");
	if(!fldv_vernum) return false;
	if(depotVer >= depot_ver::Ver5)
	{
		fldv_cversion = get_field(table_versions, "CVERSION");
		if(!fldv_cversion) return false;
	}
	fldv_snapshotcrc = get_field(table_versions, "SNAPSHOTCRC");
	if(!fldv_snapshotcrc) return false;
	fldv_snapshotmaker = get_field(table_versions, "SNAPSHOTMAKER");
	if(!fldv_snapshotmaker) return false;

	rec = new char[table_versions->get_recordlen()];
	n = 0;
	for(i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		table_versions->getrecord(i, rec);
		if(*rec) continue;
		s = fldv_vernum->get_presentation(rec, true);
		v = s.ToIntDef(0);
		if(v == ver_begin) n++;
		if(v == ver_end) n++;
		if(n >= 2) break;
	}

	if(n < 2)
	{
		msreg_m.AddMessage_("В хранилище не найдены запрошенные версии конфигурации", MessageState::Error
			, "Версия с", ver_begin
			, "Версия по", ver_end);
		delete[] rec;
		return false;
	}

	__filename = System::Ioutils::TPath::GetFullPath(_filename);

	// Определяем версию структуры конфигурации (для файла version)
	if(depotVer >= depot_ver::Ver5)
	{
		frec = rec + fldv_cversion->offset;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMajor = cv_s;
		frec += 2;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMinor = cv_s;
	}
	else
	{
		configVerMinor = 0;
		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0) configVerMajor = 6;
		else if(version == db_ver::ver8_1_0_0) configVerMajor = 106;
		else configVerMajor = 216;
	}

	delete[] rec;

	// Инициализируем таблицы HISTORY и EXTERNALS
	if(!table_history)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица HISTORY.");
		return false;
	}

	if(!table_externals)
	{
		msreg_m.AddError("В базе хранилища отсутствует таблица EXTERNALS.");
		return false;
	}

	fldh_objid = get_field(table_history, "OBJID");
	if(!fldh_objid) return false;
	fldh_vernum = get_field(table_history, "VERNUM");
	if(!fldh_vernum) return false;
	fldh_objverid = get_field(table_history, "OBJVERID");
	if(!fldh_objverid) return false;
	fldh_removed = get_field(table_history, "REMOVED");
	if(!fldh_removed) return false;
	fldh_datapacked = get_field(table_history, "DATAPACKED");
	if(!fldh_datapacked) return false;
	fldh_objdata = get_field(table_history, "OBJDATA");
	if(!fldh_objdata) return false;

	flde_objid = get_field(table_externals, "OBJID");
	if(!flde_objid) return false;
	flde_vernum = get_field(table_externals, "VERNUM");
	if(!flde_vernum) return false;
	flde_extname = get_field(table_externals, "EXTNAME");
	if(!flde_extname) return false;
	flde_extverid = get_field(table_externals, "EXTVERID");
	if(!flde_extverid) return false;
	flde_datapacked = get_field(table_externals, "DATAPACKED");
	if(!flde_datapacked) return false;
	flde_extdata = get_field(table_externals, "EXTDATA");
	if(!flde_extdata) return false;

	if(depotVer >= depot_ver::Ver6)
	{
		fldh_datahash = get_field(table_history, "DATAHASH");
		if(!fldh_datahash) return false;
		flde_datahash = get_field(table_externals, "DATAHASH");
		if(!flde_datahash) return false;

		sp = filename.SubString(1, filename.LastDelimiter("\\")) + "data\\pack\\";
		s = sp + "pack-*.ind";
		if(FindFirst(s, 0, srec) == 0)
		{
			do
			{
				try
				{
					f = new TFileStream(sp + srec.Name, fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
					msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
						"Файл", srec.Name);
					FindClose(srec);
					return false;
				}
				f->Seek(8, soFromBeginning);
				f->Read(&i, 4);
				pd.datahashes = new _datahash[i];
				f->Read(pd.datahashes, i * sizeof(_datahash));
				delete f;
				pd.count = i;
				try
				{
					s = sp + srec.Name.SubString(1, srec.Name.GetLength() - 3) + "pck";
					pd.pack = new TFileStream(s, fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
				    delete pd.pack;
				    msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
						"Файл", s);
					FindClose(srec);
					return false;
				}
				packdates.push_back(pd);
			} while(FindNext(srec) == 0);
			FindClose(srec);
		}

		sp = filename.SubString(1, filename.LastDelimiter("\\")) + "data\\objects\\";
	}
	else
	{
		fldh_datahash = NULL;
		flde_datahash = NULL;
	}

	indh = get_index(table_history, "PK");
	if(!indh) return 0;
	inde = get_index(table_externals, "PK");
	if(!inde) return 0;

	rech = new char[table_history->get_recordlen()];
	rech1 = new char[table_history->get_recordlen()];
	rech2 = new char[table_history->get_recordlen()];
	rece = new char[table_externals->get_recordlen()];
	memset(rece, 0, table_externals->get_recordlen());

	nh = indh->get_numrecords();
	ne = inde->get_numrecords();
	memset(curobj, 0, 16);


	if(configVerMajor < 100)
	{
		cath = __filename + "\\metadata\\";
	}
	else
	{
		cath = __filename + "\\";
	}

	lastver = -1;
	memset(emptyimage, 0, 8);

	sd = new TMemoryStream;
	sw = new TStreamWriter(sd, TEncoding::UTF8, 4096);
	hasdeleted = false;

	in = new TMemoryStream;
	out = new TMemoryStream;
	cat = NULL;
	hasrech1 = false;
	hasrech2 = false;
	for(ih = 0, ie = 0; ih <= nh; ih++)
	{
		if(ih < nh)
		{
			i = indh->get_numrec(ih);
			table_history->getrecord(i, rech);
		}

		if(memcmp(curobj, rech + fldh_objid->offset, 16) != 0 || ih == nh)
		{ // это новый объект или конец таблицы
			if(ih) if(hasrech2)
			{
				s = fldh_vernum->get_presentation(rech2, false);
				lastver = s.ToIntDef(MaxInt);
				sn = fldh_objid->get_presentation(rech2, false, L'.', true);

				hasext = true;
				removed = *(rech2 + fldh_removed->offset);
				if(removed)
				{
					if(hasrech1)
					{
						sw->Write(sn + "\r\n");
						hasdeleted = true;
					}
					else hasext = false;
				}
				else
				{
					datapacked = false;
					if(*(rech2 + fldh_datapacked->offset)) if(*(rech2 + fldh_datapacked->offset + 1)) datapacked = true;

					if(datapacked)
					{
						changed = true;
						inreaded = false;
						if(hasrech1)
						{
							datapacked = false;
							if(*(rech1 + fldh_datapacked->offset)) if(*(rech1 + fldh_datapacked->offset + 1)) datapacked = true;
							if(datapacked)
							{
								rec = rech1 + fldh_objdata->offset + 1;
								if(*(rech1 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
								{
									table_history->readBlob(out, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
									rec = rech2 + fldh_objdata->offset + 1;
									if(*(rech2 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
									{
										table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
										inreaded = true;
										if(in->GetSize() == out->GetSize()) if(memcmp(in->GetMemory(), out->GetMemory(), in->GetSize()) == 0) changed = false;
									}
								}
								else if(depotVer >= depot_ver::Ver6)
								{
									if(memcmp(rech2 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0)
											, rech1 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0)
											, fldh_datahash->length) == 0) changed = false;
								}
							}
						}

						if(changed)
						{
							ok = false;
							deletesobj = false;
							packlen = 0;
							rec = rech2 + fldh_objdata->offset + 1;
							if(inreaded)
							{
								sobj = in;
								ok = true;
							}
							else if(*(rech2 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
							{
								table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
								sobj = in;
								ok = true;
							}
							else if(depotVer >= depot_ver::Ver6)
							{
								rec = rech2 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0);
								for(i = 0; i < packdates.size(); i++)
								{
									pdr = &packdates[i];
									for(k = 0; k < pdr->count; k++) if(memcmp(rec, pdr->datahashes[k].datahash, 20) == 0)
									{
										sobj = pdr->pack;
										sobj->Seek(pdr->datahashes[k].offset, soBeginning);
										sobj->Read(&packlen, 8);
										in->SetSize(0);
										in->CopyFrom(sobj, packlen);
										sobj = in;
										ok = true;
										break;
									}
									if(ok) break;
								}

								if(!ok)
								{
									ss = fldh_datahash->get_presentation(rech2, true);
									s = sp + ss.SubString(1, 2) + L'\\' + ss.SubString(3, ss.GetLength() - 2);
									if(FileExists(s))
									{
										try
										{
											sobj = new TFileStream(s, fmOpenRead | fmShareDenyNone);
											deletesobj = true;
											ok = true;
										}
										catch(...)
										{
											msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
												"Файл", s,
												"Таблица", "HISTORY",
												"Объект", sn,
												"Версия", lastver);
										}
									}
									else
									{
										msreg_m.AddMessage_("Не найден файл", MessageState::Error,
											"Файл", s,
											"Таблица", "HISTORY",
											"Объект", sn,
											"Версия", lastver);
									}
								}
							}

							if(!ok)
							{
								msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
									"Таблица", "HISTORY",
									"Объект", sn,
									"Версия", lastver);
							}
							else
							{
								f = new TFileStream(cath + sn, fmCreate);
								sobj->Seek(0, soFromBeginning);
								ZInflateStream(sobj, f);
								delete f;
								if(deletesobj) delete sobj;

							}
						}
					}
				}

				// Вот тут идем по EXTERNALS
				while(hasext)
				{
					if(ie > ne) break;
					res = memcmp(rece + flde_objid->offset, curobj, 16);
					if(res > 0) break;
					if(!res)
					{
						s = flde_vernum->get_presentation(rece, false);
						v = s.ToIntDef(MaxInt);
						if(v == lastver)
						{
							se = flde_extname->get_presentation(rece);
							if(removed)
							{
								sw->Write(se + "\r\n");
								hasdeleted = true;
							}
							else
							{
								datapacked = *(rece + flde_datapacked->offset);

								// ==> Поиск записи о файле
								// В случае отсутствия данных (datapacked = false) в этой записи пытаемся найти предыдущую запись с данными
								// (с тем же objid, extname и extverid), но в пределах ver_begin <= vernum < lastver
								memcpy(verid, rece + flde_extverid->offset, 16);
								je = ie;
								while(!datapacked && v > ver_begin && je)
								{
									i = inde->get_numrec(--je);
									table_externals->getrecord(i, rece);
									res = memcmp(rece + flde_objid->offset, curobj, 16);
									if(res) break;
									s = flde_extname->get_presentation(rece);
									if(s.CompareIC(se)) continue;
									if(memcmp(verid, rece + flde_extverid->offset, 16) == 0)
									{
										s = flde_vernum->get_presentation(rece, false);
										v = s.ToIntDef(MaxInt);
										if(v < ver_begin) break;
										datapacked = *(rece + flde_datapacked->offset);
									}
								}
								// <== Поиск записи о файле

								if(datapacked)
								{
									ok = false;
									packlen = 0;
									deletesobj = false;
									frec = rece + flde_extdata->offset;
									if(memcmp(emptyimage, frec, 8))
									{
										table_externals->readBlob(in, *(uint32_t*)frec, *(uint32_t*)(frec + 4));
										sobj = in;
										ok = true;
									}
									else if(depotVer >= depot_ver::Ver6)
									{
										frec = rece + flde_datahash->offset + (flde_datahash->null_exists ? 1 : 0);
										for(i = 0; i < packdates.size(); i++)
										{
											pdr = &packdates[i];
											for(k = 0; k < pdr->count; k++) if(memcmp(frec, pdr->datahashes[k].datahash, 20) == 0)
											{
												sobj = pdr->pack;
												sobj->Seek(pdr->datahashes[k].offset, soBeginning);
												sobj->Read(&packlen, 8);
												in->SetSize(0);
												in->CopyFrom(sobj, packlen);
												sobj = in;
												ok = true;
												break;
											}
											if(ok) break;
										}

										if(!ok)
										{
											ss = flde_datahash->get_presentation(rece, true);
											s = sp + ss.SubString(1, 2) + L'\\' + ss.SubString(3, ss.GetLength() - 2);
											if(FileExists(s))
											{
												try
												{
													sobj = new TFileStream(s, fmOpenRead | fmShareDenyNone);
													deletesobj = true;
													ok = true;
												}
												catch(...)
												{
													msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
														"Файл", s,
														"Таблица", "EXTERNALS",
														"Объект", sn,
														"Файл конфигурации", se,
														"Версия", v);
												}
											}
											else
											{
												msreg_m.AddMessage_("Не найден файл", MessageState::Error,
													"Файл", s,
													"Таблица", "EXTERNALS",
													"Объект", sn,
													"Файл конфигурации", se,
													"Версия", v);
											}
										}
									}
									if(!ok)
									{
										msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
											"Таблица", "EXTERNALS",
											"Объект", sn,
											"Файл конфигурации", se,
											"Версия", v);
									}
									else
									{
										out->SetSize(0);
										sobj->Seek(0, soFromBeginning);
										ZInflateStream(sobj, out);
										iscatalog = false;
										if(out->GetSize() > 0)
										{
											cat = new v8catalog(out, false, true);
											iscatalog = cat->IsCatalog();
										}
										if(iscatalog) cat->SaveToDir(cath + se);
										else
										{
											f = new TFileStream(cath + se, fmCreate);
											f->CopyFrom(out, 0);
											delete f;
										}
										delete cat;
										cat = NULL;
										if(deletesobj) delete sobj;

									}

								}

							}
						}

					}
					if(ie == ne)
					{
						ie++;
						break;
					}
					i = inde->get_numrec(ie++);
					table_externals->getrecord(i, rece);
				}
			}

			memcpy(curobj, rech + fldh_objid->offset, 16);
			hasrech1 = false;
			hasrech2 = false;
		}

		if(ih < nh)
		{
			s = fldh_vernum->get_presentation(rech, false);
			v = s.ToIntDef(MaxInt);
			if(v < ver_begin)
			{
				memcpy(rech1, rech, table_history->get_recordlen());
				hasrech1 = true;
			}
			else if(v <= ver_end)
			{
				memcpy(rech2, rech, table_history->get_recordlen());
				hasrech2 = true;
			}
		}
	}

	delete sw;
	if(hasdeleted)
	{
		f = new TFileStream(__filename + "\\deleted", fmCreate);
		f->CopyFrom(sd, 0);
		delete f;
	}
	delete sd;

	delete[] rech;
	delete[] rech1;
	delete[] rech2;
	delete[] rece;

	for(i = 0; i < packdates.size(); i++)
	{
		delete packdates[i].pack;
		delete[] packdates[i].datahashes;
	}



	return true;
}


#ifndef PublicRelease
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
			,"Номер блока (hex)", tohex(block)
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
			,"Номер блока (hex)", tohex(block)
			,"Длина файла", l
			,"Длина записи", tab->get_recordlen()
		);
		return;
	}

	rectt = tab->get_record_template_test();

	for(i = 0, j = 0, k = 0; l; ++i)
	{
		cl = std::min(DEFAULT_PAGE_SIZE, l); // TODO pagesize

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
					s += tohex(bk[d]);
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

		if(++j >= 1023) // TODO pagesize
		{
			j = 0;
			++a;
		}
		if(l < DEFAULT_PAGE_SIZE) {
			l = 0; // TODO pagesize
		}
		else {
			l-=DEFAULT_PAGE_SIZE; // TODO pagesize
		}
	}


	delete[] rectt;

	flush();
}

//---------------------------------------------------------------------------
// Проверка блока таблицы по шаблону
bool T_1CD::test_block_by_template(uint32_t testblock, char* tt, uint32_t num, int32_t rlen, int32_t len)
{
	unsigned char b[DEFAULT_PAGE_SIZE]; // TODO pagesize
	bool ok;
	int32_t i, j;

	ok = true;
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
			return false; // TODO pagesize
		}
	}
	return true;
}

#endif //#ifdef PublicRelease

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
bool T_1CD::save_config_ext(const String& _filename, const TGUID& uid, const String& hashname)
{
	ConfigStorageTableConfigCasSave* cs;
	bool res;

	cs = new ConfigStorageTableConfigCasSave(get_files_configcas(), get_files_configcassave(), uid, hashname);
	if(!cs->getready()) {
		res = false;
	}
	else {
		res = cs->save_config(_filename);
	}
	delete cs;
	return res;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config_ext_db(const String& _filename, const String& hashname)
{
	ConfigStorageTableConfigCas* cs;
	bool res;

	cs = new ConfigStorageTableConfigCas(get_files_configcas(), hashname);
	if(!cs->getready()) res = false;
	else res = cs->save_config(_filename);
	delete cs;
	return res;

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



