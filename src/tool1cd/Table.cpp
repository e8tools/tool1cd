/*
 *
 *
 *
 *
 */

#include <string>
#include <boost/filesystem.hpp>

#include "Table.h"
#include "TableRecord.h"
#include "Common.h"

extern Registrator msreg_g;
#ifndef getcfname
extern TMultiReadExclusiveWriteSynchronizer* tr_syn;
#endif

using namespace std;

//********************************************************
// Класс changed_rec

//---------------------------------------------------------------------------
changed_rec::changed_rec(Table* _parent, changed_rec_type crt, uint32_t phys_numrecord)
{
	parent = _parent;
	numrec = phys_numrecord;
	changed_type = crt;
	if(crt == changed_rec_type::deleted)
	{
		fields = nullptr;
		rec = nullptr;
	}
	else
	{
		fields = new char[parent->num_fields];
		memset(fields, 0, parent->num_fields);
		rec = new char[parent->recordlen];
		memset(rec, 0, parent->recordlen);
	}
	next = parent->ch_rec;
	parent->ch_rec = this;
}
//---------------------------------------------------------------------------
changed_rec::~changed_rec()
{
	clear();
	delete[] fields;
	delete[] rec;
}

//---------------------------------------------------------------------------
void changed_rec::clear()
{
	int32_t i;
	Field* f;
	type_fields tf;
	TStream* b;

	if(rec && fields) for(i = 0; i < parent->num_fields; i++) if(fields[i])
	{
		f = parent->fields[i];
		tf = f->gettype();
		if(tf == type_fields::tf_image || tf == type_fields::tf_string || tf == type_fields::tf_text)
		{
			b = *(TStream**)(rec + f->getoffset() + (f->getnull_exists() ? 1 : 0));
			delete b;
		}
	}

}



//********************************************************
// Класс table

//---------------------------------------------------------------------------
bool Table::get_issystem()
{
	return issystem;
}

void Table::init()
{
	num_fields = 0;
	fields.clear();
	num_indexes = 0;
	indexes.clear();
	recordlock = false;
	file_data = nullptr;
	file_blob = nullptr;
	file_index = nullptr;
	lockinmemory = 0;

	recordsindex_complete = false;
	numrecords_review = 0;
	numrecords_found = 0;

	edit = false;
	ch_rec = nullptr;
	added_numrecords = 0;

	phys_numrecords = 0;
	log_numrecords = 0;
	bad = true;
}

class TableReadError : public DetailedException
{
public:
	TableReadError(const string &message, int32_t block_descr)
			: DetailedException(message)
	{
		add_detail("Блок", to_hex_string(block_descr));
	}

	TableReadError(const string &message, int32_t block_descr, const string &table_name)
			: DetailedException(message)
	{
		add_detail("Блок", to_hex_string(block_descr));
		add_detail("Таблица", table_name);
	}
};

//---------------------------------------------------------------------------
void Table::init(int32_t block_descr)
{
	tree* t;
	tree* f;
	tree* rt;
	int32_t j;
	uint32_t m;
	uint64_t s;
	int32_t blockfile[3];
	uint32_t* buf;

	init();

	if (description.empty()) {
		return;
	}

	std::unique_ptr<tree> root(parse_1Ctext(description, string("Блок ") + to_string(block_descr)));

	if (!root) {
		throw TableReadError("Ошибка разбора текста описания таблицы.", block_descr);
	}

	if (root->get_num_subnode() != 1) {
		throw TableReadError("Ошибка разбора текста описания таблицы. Количество узлов не равно 1.", block_descr)
				.add_detail("Узлов", root->get_num_subnode());
	}
	rt = root->get_first();

	if (rt->get_num_subnode() != 6) {
		throw TableReadError("Ошибка разбора текста описания таблицы. Количество узлов не равно 6.", block_descr)
				.add_detail("Узлов", rt->get_num_subnode());
	}

	t = rt->get_first();
	if (t->get_type() != node_type::nd_string) {
		throw TableReadError("Ошибка получения имени таблицы. Узел не является строкой.", block_descr);
	}
	this->name = t->get_value();
	this->issystem = name[0] != '_'
		|| EndsWithIC(name, "STORAGE")
		|| EqualIC(name, "_SYSTEMSETTINGS")
		|| EqualIC(name, "_COMMONSETTINGS")
		|| EqualIC(name, "_REPSETTINGS")
		|| EqualIC(name, "_REPVARSETTINGS")
		|| EqualIC(name, "_FRMDTSETTINGS")
		|| EqualIC(name, "_SCHEDULEDJOBS");

	t = t->get_next();
	// пропускаем узел, так как там всегда содержится "0", и что это такое, неизвестно (версия формата описания таблиц?)
	t = t->get_next();
	if (t->get_type() != node_type::nd_list) {
		throw TableReadError("Ошибка получения полей таблицы. Узел не является деревом.", block_descr, name);
	}
	if(t->get_num_subnode() < 2) {
		throw TableReadError("Ошибка получения полей таблицы. Нет узлов описания полей.", block_descr, name);
	}

	num_fields = t->get_num_subnode() - 1;
	fields.reserve(num_fields);
	bool has_version = false; // признак наличия поля версии

	f = t->get_first();
	if(f->get_type() != node_type::nd_string) {
		throw TableReadError("Ошибка получения полей таблицы. Ожидаемый узел Fields не является строкой.", block_descr, name);
	}
	if (f->get_value() != "Fields") {
		throw TableReadError("Ошибка получения полей таблицы. Узел не Fields.", block_descr, name)
				.add_detail("Узел", f->get_value());
	}

	for(int32_t i = 0; i < num_fields; i++)
	{
		f = f->get_next();
		if(f->get_num_subnode() != 6) {
			throw TableReadError("Ошибка получения узла очередного поля таблицы. "
										 "Количество узлов поля не равно 6.", block_descr, name)
					.add_detail("Номер поля", i + 1)
					.add_detail("Узлов", f->get_num_subnode());
		}

		tree *field_tree = f->get_first();
		try {

			fields.push_back(Field::field_from_tree(field_tree, has_version, this));

		} catch (FieldStreamParseException &formatError) {
			throw formatError
					.add_detail("Блок", to_hex_string(block_descr))
					.add_detail("Таблица", name)
					.add_detail("Номер поля", i + 1);
		}
	}

	t = t->get_next();
	if (t->get_type() != node_type::nd_list) {
		throw TableReadError("Ошибка получения индексов таблицы. Узел не является деревом.", block_descr, name);
	}
	if (t->get_num_subnode() < 1) {
		throw TableReadError("Ошибка получения индексов таблицы. Нет узлов описания индексов.", block_descr, name);
	}

	num_indexes = t->get_num_subnode() - 1;
	if (num_indexes) {

		f = t->get_first();
		if(f->get_type() != node_type::nd_string) {
			throw TableReadError("Ошибка получения индексов таблицы. Ожидаемый узел Indexes не является строкой.", block_descr, name);
		}
		if (f->get_value() != "Indexes") {
			throw TableReadError("Ошибка получения индексов таблицы. Узел не Indexes.", block_descr, name)
					.add_detail("Узел", f->get_value());
		}

		for (int i = 0; i < num_indexes; i++) {
			f = f->get_next();
			try {

				indexes.push_back(Index::index_from_tree(f, this));

			} catch (DetailedException &err) {
				err.add_detail("Номер блока", to_hex_string(block_descr));
				err.add_detail("Таблица", name);
				err.add_detail("Номер индекса", i + 1);
				throw err;
			}
		}
	}

	t = t->get_next();
	if (t->get_num_subnode() != 2) {
		throw TableReadError("Ошибка получения типа блокировки таблицы. Количество узлов не равно 2.", block_descr, name);
	}

	f = t->get_first();
	if (f->get_type() != node_type::nd_string) {
		throw TableReadError("Ошибка получения типа блокировки таблицы. Ожидаемый узел Recordlock не является строкой.", block_descr, name);
	}
	if (f->get_value() != "Recordlock") {
		throw TableReadError("Ошибка получения типа блокировки таблицы. Узел не Recordlock.", block_descr, name)
				.add_detail("Узел", f->get_value());
	}

	f = f->get_next();
	if (f->get_type() != node_type::nd_string) {
		throw TableReadError("Ошибка получения типа блокировки таблицы. Узел не является строкой.", block_descr, name);
	}
	std::string sTableLock = f->get_value();
	if     (sTableLock == "0") recordlock = false;
	else if(sTableLock == "1") recordlock = true;
	else {
		throw TableReadError("Неизвестное значение типа блокировки таблицы.", block_descr, name)
				.add_detail("Тип блокировки", sTableLock);
	}

	if (recordlock && !has_version) {
		// добавляем скрытое поле версии
		Field *fld = new Field(this);
		fld->name = "VERSION";
		fld->type_manager = FieldType::Version8();
		fields.push_back(fld);
	}

	t = t->get_next();
	if(t->get_num_subnode() != 4) {
		throw TableReadError("Ошибка получения файлов таблицы. Количество узлов не равно 4.", block_descr, name);
	}

	f = t->get_first();
	if(f->get_type() != node_type::nd_string) {
		throw TableReadError("Ошибка получения файлов таблицы. Ожидаемый узел Files не является строкой.", block_descr, name);
	}
	if (f->get_value() != "Files") {
		throw TableReadError("Ошибка получения файлов таблицы. Узел не Files.", block_descr, name)
				.add_detail("Узел", f->get_value());
	}

	for(int32_t i = 0; i < 3; i++)
	{
		f = f->get_next();
		if (f->get_type() != node_type::nd_number) {
			throw TableReadError("Ошибка получения файла таблицы. Узел не является числом.", block_descr, name)
					.add_detail("Номер файла", i + 1);
		}
		blockfile[i] = stoi(f->get_value());
	}

	if (blockfile[0]) {
		file_data = new v8object(base, blockfile[0]);
	}
	if (blockfile[1]) {
		file_blob = new v8object(base, blockfile[1]);
	}
	if (blockfile[2]) {
		file_index = new v8object(base, blockfile[2]);
	}

	if(num_indexes && !file_index)
	{
		throw DetailedException("В таблице есть индексы, однако файл индексов отсутствует.")
			.add_detail("Блок", to_hex_string(block_descr))
			.add_detail("Таблица", name)
			.add_detail("Количество индексов", num_indexes);
	}
	else if(!num_indexes && file_index)
	{
		throw DetailedException("В таблице нет индексов, однако присутствует файл индексов.")
			.add_detail("Блок", to_hex_string(block_descr))
			.add_detail("Таблица", name)
			.add_detail("Блок индексов", to_hex_string(blockfile[2]));
	}
	if(file_index)
	{
		m = file_index->getlen() / base->pagesize;
		if(file_index->getlen() != m * base->pagesize)
		{
			throw DetailedException("Ошибка чтения индексов. Длина файла индексов не кратна размеру страницы")
				.add_detail("Таблица", name)
				.add_detail("Длина файла индексов", to_hex_string(file_index->getlen()));
		}
		{
			int32_t buflen = num_indexes * 4 + 4;
			buf = new uint32_t[num_indexes + 1];
			file_index->getdata(buf, 0, buflen);

			if(buf[0] * base->pagesize >= file_index->getlen())
			{
				throw DetailedException("Ошибка чтения индексов. Индекс первого свободного блока за пределами файла индексов")
					.add_detail("Таблица", name)
					.add_detail("Длина файла индексов", to_hex_string(file_index->getlen()))
					.add_detail("Индекс свободной страницы", to_hex_string(buf[0]));
			}
			{
				for(int32_t i = 1; i <= num_indexes; i++)
				{
					if(base->version < db_ver::ver8_3_8_0)
					{
						if(buf[i] >= file_index->getlen())
						{
							throw DetailedException("Ошибка чтения индексов. Указанное смещение индекса за пределами файла индексов")
								.add_detail("Таблица", name)
								.add_detail("Длина файла индексов", to_hex_string(file_index->getlen()))
								.add_detail("Номер индекса", i)
								.add_detail("Смещение индекса", to_hex_string(buf[i]));
						}
						if(buf[i] & 0xfff)
						{
							throw DetailedException("Ошибка чтения индексов. Указанное смещение индекса не кратно 4 Кб")
								.add_detail("Таблица", name)
								.add_detail("Длина файла индексов", to_hex_string(file_index->getlen()))
								.add_detail("Номер индекса", i)
								.add_detail("Смещение индекса", to_hex_string(buf[i]));
						}
						indexes[i - 1]->start = buf[i];
					}
					else
					{
						s = buf[i];
						s *= base->pagesize;
						if(s >= file_index->getlen())
						{
							throw DetailedException("Ошибка чтения индексов. Указанное смещение индекса за пределами файла индексов")
								.add_detail("Таблица", name)
								.add_detail("Длина файла индексов", file_index->getlen())
								.add_detail("Номер индекса", i)
								.add_detail("Смещение индекса", s);
						}
						indexes[i - 1]->start = s;
					}
				}
			}

			delete[] buf;

		}

	}

	// вычисляем длину записи таблицы как сумму длинн полей и проставим смещения полей в записи
	recordlen = 1; // первый байт записи - признак удаленности
	// сначала идут поля (поле) с типом "версия"
	for(int32_t i = 0; i < num_fields; i++) {
		if (fields[i]->type_manager->gettype() == type_fields::tf_version
			|| fields[i]->type_manager->gettype() == type_fields::tf_version8) {
			fields[i]->offset = recordlen;
			recordlen += fields[i]->getlen();
		}
	}
	// затем идут все остальные поля
	for(int32_t i = 0; i < num_fields; i++) {
		if (fields[i]->type_manager->gettype() != type_fields::tf_version
			&& fields[i]->type_manager->gettype() != type_fields::tf_version8) {
			fields[i]->offset = recordlen;
			recordlen += fields[i]->getlen();
		}
	}
	if(recordlen < 5) recordlen = 5; // Длина одной записи не может быть меньше 5 байт (1 байт признак, что запись свободна, 4 байт - индекс следующей следующей свободной записи)

	if(!recordlen || !file_data) phys_numrecords = 0;
	else phys_numrecords = file_data->getlen() / recordlen;;

	if(file_data)
	{
		if(phys_numrecords * recordlen != file_data->getlen())
		{
			throw DetailedException("Длина таблицы не кратна длине записи.")
				.add_detail("Блок", to_hex_string(block_descr))
				.add_detail("Таблица", name)
				.add_detail("Длина таблицы", file_data->getlen())
				.add_detail("Длина записи", recordlen);
		}
	}
	else
	{
		throw DetailedException("Отсутствует файл данных таблицы.")
			.add_detail("Блок", to_hex_string(block_descr))
			.add_detail("Таблица", name);
	}

	// Инициализация данных индекса
	for(int32_t i = 0; i < num_indexes; i++) indexes[i]->get_length();

	bad = false;

}

//---------------------------------------------------------------------------
Table::Table(T_1CD* _base, int32_t block_descr)
{

	base = _base;

	descr_table = new v8object(base, block_descr);
	auto data = descr_table->getdata();
	description = TEncoding::Unicode->toUtf8(vector<uint8_t>(data, data + descr_table->getlen()));

	try {

		init(block_descr);

	} catch (DetailedException &err) {
		init();
		throw err;
	}
}

//---------------------------------------------------------------------------
Table::Table(T_1CD *_base, const std::string &_descr, int32_t block_descr)
{

	base = _base;

	descr_table = 0;
	description = _descr;

	try {

		init(block_descr);

	} catch (DetailedException &err) {
		init();
		throw err;
	}
}

//---------------------------------------------------------------------------
Table::Table()
{
	init();
}

//---------------------------------------------------------------------------
void Table::deletefields()
{
	fields.clear();
}

//---------------------------------------------------------------------------
void Table::deleteindexes()
{
	for (auto index : indexes) {
		delete index;
	}
}

//---------------------------------------------------------------------------
Table::~Table()
{
	changed_rec* cr;
	changed_rec* cr2;
	for(cr = ch_rec; cr;)
	{
		cr2 = cr->next;
		delete cr;
		cr = cr2;
	}

	deletefields();
	deleteindexes();
	if(file_data)
	{
		delete file_data;
		file_data = nullptr;
	}
	if(file_blob)
	{
		delete file_blob;
		file_blob = nullptr;
	}
	if(file_index)
	{
		delete file_index;
		file_index = nullptr;
	}
	if(descr_table)
	{
		delete descr_table;
		descr_table = nullptr;
	}

}

//---------------------------------------------------------------------------
std::string Table::getname() const
{
	return name;
}

//---------------------------------------------------------------------------
std::string Table::getdescription() const
{
	return description;
}

//---------------------------------------------------------------------------
int32_t Table::get_numfields() const
{
	return num_fields;
}

//---------------------------------------------------------------------------
int32_t Table::get_numindexes() const
{
	return num_indexes;
}

//---------------------------------------------------------------------------
Field* Table::getfield(int32_t numfield)
{
	if(numfield >= num_fields)
	{
		throw DetailedException("Попытка получения поля таблицы по номеру, превышающему количество полей")
			.add_detail("Таблица", name)
			.add_detail("Количество полей", num_fields)
			.add_detail("Номер поля", numfield + 1);
	}
	return fields[numfield];
}

//---------------------------------------------------------------------------
Index* Table::getindex(int32_t numindex)
{
	if(numindex >= num_indexes)
	{
		throw DetailedException("Попытка получения индекса таблицы по номеру, превышающему количество индексов")
			.add_detail("Таблица", name)
			.add_detail("Количество индексов", num_indexes)
			.add_detail("Номер индекса", numindex + 1);
	}
	return indexes[numindex];
}

//---------------------------------------------------------------------------
uint32_t Table::get_phys_numrecords()
{
	return phys_numrecords;
}

//---------------------------------------------------------------------------
uint32_t Table::get_log_numrecords()
{
	return log_numrecords;
}

//---------------------------------------------------------------------------
void Table::set_log_numrecords(uint32_t _log_numrecords)
{
	log_numrecords = _log_numrecords;
}

//---------------------------------------------------------------------------
uint32_t Table::get_added_numrecords()
{
	return added_numrecords;
}

//---------------------------------------------------------------------------
void Table::getrecord(uint32_t phys_numrecord, char *buf)
{
	file_data->getdata(buf, phys_numrecord * recordlen, recordlen);
}

//---------------------------------------------------------------------------
TableRecord * Table::getrecord(uint32_t phys_numrecord)
{
	#ifndef getcfname
	tr_syn->BeginWrite();
	#endif
	char *buf = new char[recordlen];
	char* b = file_data->getdata(buf, phys_numrecord * recordlen, recordlen);
	#ifndef getcfname
	tr_syn->EndWrite();
	#endif
	return new TableRecord(this, b, recordlen);
}

//---------------------------------------------------------------------------
int32_t Table::get_recordlen() const
{
	return recordlen;
}

//---------------------------------------------------------------------------
bool Table::get_recordlock()
{
	return recordlock;
}

//---------------------------------------------------------------------------
v8object* Table::get_file_data()
{
	return file_data;
}

//---------------------------------------------------------------------------
v8object* Table::get_file_blob()
{
	return file_blob;
}

//---------------------------------------------------------------------------
v8object* Table::get_file_index()
{
	return file_index;
}

//---------------------------------------------------------------------------
void Table::set_lockinmemory(bool _lock)
{
	if(_lock)
	{
		lockinmemory++;
		if(descr_table) descr_table->set_lockinmemory(true);
		if(file_data) file_data->set_lockinmemory(true);
		if(file_blob) file_blob->set_lockinmemory(true);
		if(file_index) file_index->set_lockinmemory(true);
	}
	else
	{
		if(lockinmemory > 0) lockinmemory--;
		if(!lockinmemory)
		{
			if(descr_table) descr_table->set_lockinmemory(false);
			if(file_data) file_data->set_lockinmemory(false);
			if(file_blob) file_blob->set_lockinmemory(false);
			if(file_index) file_index->set_lockinmemory(false);
		}
	}
}

//---------------------------------------------------------------------------
// rewrite - перезаписывать поток _str. Истина - перезаписывать (по умолчанию), Ложь - дописывать
TStream* Table::readBlob(TStream* _str, uint32_t _startblock, uint32_t _length, bool rewrite) const
{
	uint32_t _curblock;
	char* _curb;
	uint16_t _curlen;
	uint32_t _filelen, _numblock;
	uint32_t startlen;

	if(rewrite) _str->SetSize(0);
	startlen = _str->GetPosition();

	if(!_startblock && _length)
	{
		throw DetailedException("Попытка чтения нулевого блока файла Blob")
			.add_detail("Таблица", name);
	}

	if(file_blob)
	{
		_filelen = file_blob->getlen();
		_numblock = _filelen >> 8;
		if(_numblock << 8 != _filelen)
			throw DetailedException("Длина файла Blob не кратна 0x100")
				.add_detail("Таблица", name)
				.add_detail("Длина файла", to_hex_string(_filelen));

		_curb = new char[0x100];
		_curblock = _startblock;
		while(_curblock)
		{
			if(_curblock >= _numblock)
			{
				throw DetailedException("Попытка чтения блока файла Blob за пределами файла")
					.add_detail("Таблица", name)
					.add_detail("Всего блоков", _numblock)
					.add_detail("Читаемый блок", _curblock);
			}
			file_blob->getdata(_curb, _curblock << 8, BLOB_RECORD_LEN);
			_curblock = *(uint32_t*)_curb;
			_curlen = *(uint16_t*)(_curb + 4);
			if(_curlen > BLOB_RECORD_DATA_LEN)
			{
				throw DetailedException("Попытка чтения из блока файла Blob более 250 байт")
					.add_detail("Таблица", name)
					.add_detail("Индекс блока", _curblock)
					.add_detail("Читаемых байт", _curlen);
			}
			_str->Write(_curb + 6, _curlen);

			if(_str->GetSize() - startlen > _length) break; // аварийный выход из возможного ошибочного зацикливания
		}
		delete[] _curb;

		if(_str->GetSize() - startlen != _length)
		{
			throw DetailedException("Несовпадение длины Blob-поля, указанного в записи, с длиной практически прочитанных данных")
				.add_detail("Таблица", name)
				.add_detail("Длина поля", _length)
				.add_detail("Прочитано", _str->GetSize() - startlen);
		}
	}
	else
	{
		throw DetailedException("Попытка чтения Blob-поля при отстутствующем файле Blob")
			.add_detail("Таблица", name)
			.add_detail("Длина поля", _length);
	}

	return _str;
}

//---------------------------------------------------------------------------
uint32_t Table::readBlob(void* buf, uint32_t _startblock, uint32_t _length) const
{
	uint32_t _curblock;
	char* _curb;
	char* _buf;
	uint16_t _curlen;
	uint32_t _filelen, _numblock;
	uint32_t readed;

	if(!_startblock && _length)
	{
		throw DetailedException("Попытка чтения нулевого блока файла Blob")
			.add_detail("Таблица", name);
	}

	_buf = (char*)buf;
	readed = 0;
	if(file_blob)
	{
		_filelen = file_blob->getlen();
		_numblock = _filelen >> 8;
		if(_numblock << 8 != _filelen) {
			throw DetailedException("Длина файла Blob не кратна 0x100")
					.add_detail("Таблица", name)
					.add_detail("Длина файла", to_hex_string(_filelen));
		}

		_curb = new char[0x100];
		_curblock = _startblock;
		while(_curblock)
		{
			if(_curblock >= _numblock)
			{
				throw DetailedException("Попытка чтения блока файла Blob за пределами файла")
					.add_detail("Таблица", name)
					.add_detail("Всего блоков", _numblock)
					.add_detail("Читаемый блок", _curblock);
			}
			file_blob->getdata(_curb, _curblock << 8, BLOB_RECORD_LEN);
			_curblock = *(uint32_t*)_curb;
			_curlen = *(uint16_t*)(_curb + 4);
			if(_curlen > BLOB_RECORD_DATA_LEN)
			{
				throw DetailedException("Попытка чтения из блока файла Blob более 250 байт")
					.add_detail("Таблица", name)
					.add_detail("Индекс блока", _curblock)
					.add_detail("Читаемых байт", _curlen);
			}
			memcpy(_buf, _curb + 6, _curlen);
			_buf += _curlen;
			readed += _curlen;

			if(readed > _length) break; // аварийный выход из возможного ошибочного зацикливания
		}
		delete[] _curb;

		if(readed != _length)
		{
			throw DetailedException("Несовпадение длины Blob-поля, указанного в записи, с длиной практически прочитанных данных")
				.add_detail("Таблица", name)
				.add_detail("Длина поля", _length)
				.add_detail("Прочитано", readed);
		}
	}
	else
	{
		throw DetailedException("Попытка чтения Blob-поля при отстутствующем файле Blob")
			.add_detail("Таблица", name)
			.add_detail("Длина поля", _length);
	}

	return readed;
}

//---------------------------------------------------------------------------
bool Table::export_to_xml(const std::string &_filename, bool blob_to_file, bool unpack)
{
	string recname;
	uint32_t j, numr, nr;
	bool canwriteblob = false;
	Index* curindex = nullptr;
	int32_t repeat_count; // количество повторов имени записи подряд (для случая, если индекс не уникальный)

	char UnicodeHeader[3] = {'\xef', '\xbb', '\xbf'}; // BOM UTF-8
	string part1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<!--Файл сформирован программой Tool_1CD-->\r\n<Table Name=\"";
	string part2 = "\">\r\n\t<Fields>\r\n";
	string part3 = "\t</Fields>\r\n\t<Records>\r\n";
	string part4 = "\t</Records>\r\n</Table>\r\n";

	string fpart1 = "\t\t<Field Name=\"";
	string fpart2 = "\" Type=\"";
	string fpart3 = "\" Length=\"";
	string fpart4 = "\" Precision=\"";
	string fpart6 = "\" NotNull=\"";
	string fpart5 = "\"/>\r\n";

	string rpart1 = "\t\t<Record>\r\n";
	string rpart2 = "\t\t</Record>\r\n";
	string rpart3 = "\t\t\t<";
	string status = "Экспорт таблицы ";
	status += name;
	status += " ";

	TFileStream f(boost::filesystem::path(_filename), fmCreate);

	f.Write(UnicodeHeader, 3);
	f.WriteString(part1);
	f.WriteString(name);
	f.WriteString(part2);

	auto primary = std::find_if(indexes.begin(), indexes.end(),
	                           [](Index *index) { return index->get_is_primary();});
	if (primary != indexes.end()) {
		curindex= *primary;
	}

	int image_count; // количество полей с типом image
	for (auto field : fields) {
		f.WriteString(fpart1);
		f.WriteString(field->getname());
		f.WriteString(fpart2);
		f.WriteString(field->get_presentation_type());
		f.WriteString(fpart3);
		f.WriteString(to_string(field->getlength()));
		f.WriteString(fpart4);
		f.WriteString(to_string(field->getprecision()));
		f.WriteString(fpart6);
		f.WriteString(((field->getnull_exists()) ? "false" : "true"));
		f.WriteString(fpart5);

		if (field->gettype() == type_fields::tf_image) {
			image_count++;
		}
	}

	f.WriteString(part3);

	if(curindex) numr = curindex->get_numrecords();
	else numr = numrecords_found;

	msreg_g.Status(status);

	recname = "";
	repeat_count = 0;
	bool dircreated = false;
	boost::filesystem::path dir(_filename + ".blob");

	for(j = 0; j < numr; j++)
	{
		if (j % 100 == 0 && j) {
			msreg_g.Status(status + to_string(j));
		}

		f.Write(rpart1.c_str(), rpart1.size());
		if(curindex) nr = curindex->get_numrec(j);
		else nr = recordsindex[j];
		std::shared_ptr<TableRecord> rec (getrecord(nr));
		if (image_count) {
			std::string filename = get_file_name_for_record(rec.get());
			if (EqualIC(filename, recname)) {
				repeat_count++;
			} else {
				recname = filename;
				repeat_count = 0;
			}
		}
		for (auto field : fields) {
			string outputvalue;
			f.WriteString(rpart3);
			f.WriteString(field->getname());

			bool output_is_null = false;

			if (blob_to_file && field->type_manager->gettype() == type_fields::tf_image) {
				if(!dircreated) {
					try
					{
						if(!directory_exists(dir, true))
						{
							dircreated = false;
							canwriteblob = false;
						} else {
							dircreated = true;
							canwriteblob = true;
						}
					}
					catch(...) {
						msreg_g.AddMessage("Не удалось создать каталог blob", MessageState::Warning)
							.with("Таблица", name)
							.with("Путь", dir.string());
					}
				}

				if(canwriteblob)
				{
					outputvalue = recname;
					if(image_count > 1)
					{
						if (outputvalue.size()) {
							outputvalue += "_";
						}
						outputvalue += field->name;
					}
					if(repeat_count)
					{
						outputvalue += "_";
						outputvalue += repeat_count + 1;
					}

					dir /= outputvalue;
					if(!field->save_blob_to_file(rec.get(), dir.string(), unpack)) {
						outputvalue = "{NULL}";
						output_is_null = true;
					}
				}
				else outputvalue = "{ERROR}";
			}
			else {
				if (rec->is_null_value(field)) {
					outputvalue = "{NULL}";
					output_is_null = true;
				} else {
					outputvalue = rec->get_xml_string(field);
				}
			}

			if (output_is_null) {
				f.WriteString("/>\r\n");
			} else {
				f.WriteString(">");
				f.WriteString(outputvalue);
				f.WriteString("</");
				f.WriteString(field->getname());
				f.WriteString(">\r\n");
			}
		}
		f.WriteString(rpart2);

	}
	f.WriteString(part4);

	msreg_g.Status("");
	return true;
}

//---------------------------------------------------------------------------
uint64_t Table::get_fileoffset(uint32_t phys_numrecord)
{
	uint32_t _offset = phys_numrecord * recordlen;
	return file_data->get_fileoffset(_offset);
}

//---------------------------------------------------------------------------
bool Table::get_edit()
{
	return edit;
}
//---------------------------------------------------------------------------

void Table::begin_edit()
{
	if(edit) return;
	if(base->readonly)
	{
		throw DetailedException("Попытка входа в режим редактирования в режиме \"Только чтение\"")
			.add_detail("Таблица", name);
	}
	edit = true;
}

//---------------------------------------------------------------------------
changed_rec_type Table::get_rec_type(uint32_t phys_numrecord)
{
	changed_rec* cr;
	if (!edit) {
		return changed_rec_type::not_changed;
	}
	cr = ch_rec;
	while(cr)
	{
		if(cr->numrec == phys_numrecord) return cr->changed_type;
		cr = cr->next;
	}
	return changed_rec_type::not_changed;
}

//---------------------------------------------------------------------------
changed_rec_type Table::get_rec_type(uint32_t phys_numrecord, int32_t numfield)
{
	changed_rec* cr;
	if (!edit) {
		return changed_rec_type::not_changed;
	}
	cr = ch_rec;
	while(cr) {
		if(cr->numrec == phys_numrecord) {
			if (cr->changed_type == changed_rec_type::changed) {
				return cr->fields[numfield] ? changed_rec_type::changed : changed_rec_type::not_changed;
			}
			return cr->changed_type;
		}
		cr = cr->next;
	}
	return changed_rec_type::not_changed;
}

//---------------------------------------------------------------------------
void Table::export_table(const std::string &path) const
{

	boost::filesystem::path dir(path);

	dir /= name;
	if(!directory_exists(dir, true)) {
		return;
	}

	export_import_table_root root;
	if(file_data) {
		root.has_data = true;
		root.data_version_1 = file_data->version.version_1;
		root.data_version_2 = file_data->version.version_2;
	}
	else root.has_data = false;

	if(file_blob) {
		root.has_blob = true;
		root.blob_version_1 = file_blob->version.version_1;
		root.blob_version_2 = file_blob->version.version_2;
	}
	else root.has_blob = false;

	if(file_index) {
		root.has_index = true;
		root.index_version_1 = file_index->version.version_1;
		root.index_version_2 = file_index->version.version_2;
	}
	else root.has_index = false;

	if(descr_table) {
		root.has_descr = true;
		root.descr_version_1 = descr_table->version.version_1;
		root.descr_version_2 = descr_table->version.version_2;
	}
	else root.has_descr = false;

	try {
		TFileStream f(dir / "root", fmCreate|fmShareDenyNone);
		f.Write(&root, sizeof(export_import_table_root));
	} catch(...) {
	}

	if (file_data) {
		file_data->savetofile((dir / "data").string());
	}
	if (file_blob) {
		file_blob->savetofile((dir / "blob").string());
	}
	if (file_index) {
		file_index->savetofile((dir / "index").string());
	}
	if (descr_table) {
		descr_table->savetofile((dir / "descr").string());
	}

}

//---------------------------------------------------------------------------
void Table::import_table(const std::string &path)
{
	boost::filesystem::path dir(path);
	dir /= name;

	if(!directory_exists(dir)) {
		return;
	}

	TFileStream* f;
	bool fopen;
	v8ob* ob;

	bool descr_changed = false;

	try
	{
		f = new TFileStream(dir / "root", fmOpenRead);
	}
	catch(...)
	{
		throw DetailedException("Ошибка открытия файла импорта таблицы root")
			.add_detail("Таблица", name)
			.add_detail("Файл", (dir / "root").string());
	}
	export_import_table_root root;
	f->Read(&root, sizeof(export_import_table_root));
	delete f;

	if(root.has_data) {
		fopen = false;
		try
		{
			f = new TFileStream(dir / "data", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы data")
					.add_detail("Таблица", name)
					.add_detail("Файл", (dir / "data").string());
		}
		if(fopen)
		{
			if(!file_data){
				file_data = new v8object(base);
				descr_changed = true;
			}
			file_data->setdata(f);
			ob = (v8ob*)base->getblock_for_write(file_data->block, true);
			ob->version.version_1 = root.data_version_1;
			ob->version.version_2 = root.data_version_2;
			delete f;
		}
	}

	if(root.has_blob) {
		fopen = false;
		try
		{
			f = new TFileStream(dir / "blob", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы blob")
					.add_detail("Таблица", name)
					.add_detail("Файл", (dir / "blob").string());
		}
		if(fopen)
		{
			if(!file_blob){
				file_blob = new v8object(base);
				descr_changed = true;
			}
			file_blob->setdata(f);
			ob = (v8ob*)base->getblock_for_write(file_blob->block, true);
			ob->version.version_1 = root.blob_version_1;
			ob->version.version_2 = root.blob_version_2;
			delete f;
		}
	}

	if(root.has_index) {
		fopen = false;
		try
		{
			f = new TFileStream(dir / "index", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы index")
					.add_detail("Таблица", name)
					.add_detail("Файл", (dir / "index").string());
		}
		if(fopen)
		{
			if(!file_index){
				file_index = new v8object(base);
				descr_changed = true;
			}
			file_index->setdata(f);
			ob = (v8ob*)base->getblock_for_write(file_index->block, true);
			ob->version.version_1 = root.index_version_1;
			ob->version.version_2 = root.index_version_2;
			delete f;
		}
	}

	if (descr_changed && root.has_descr)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir / "descr", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			throw DetailedException("Ошибка открытия файла импорта таблицы descr")
					.add_detail("Таблица", name)
					.add_detail("Файл", (dir / "descr").string());
		}
		if(fopen)
		{
			if(!descr_table) descr_table = new v8object(base); // вообще, если descr_table == nullptr, то это огромная ошибка!
			ob = (v8ob*)base->getblock_for_write(descr_table->get_block_number(), true);
			ob->version.version_1 = root.descr_version_1;
			ob->version.version_2 = root.descr_version_2;

			auto buf_size = f->GetSize();
			char *buf = new char[buf_size];
			f->Read(buf, buf_size);
			string str = TEncoding::Unicode->toUtf8(vector<uint8_t>(buf, buf + buf_size));
			delete[] buf;
			delete f;

			auto pos_files = str.find("{\"Files\",");
			if (pos_files == string::npos) {
				throw DetailedException("Ошибка поиска раздела Files в файле импорта таблицы descr")
					.add_detail("Файл", (dir / "descr").string());
			}
			str.resize(pos_files);
			str += "{\"Files\",";
			str += file_data ? to_string(file_data->get_block_number()) : string("0");
			str += ",";
			str += file_blob ? to_string(file_blob->get_block_number()) : string("0");
			str += ",";
			str += file_index ? to_string(file_index->get_block_number()) : string("0");
			str += "}\n}";

			auto data = TEncoding::Unicode->fromUtf8(str);
			descr_table->setdata(data.data(), data.size());

		}

	}

	base->flush();

}

//---------------------------------------------------------------------------
void Table::set_edit_value(uint32_t phys_numrecord, int32_t numfield, bool null, const std::string &value, TStream *st)
{
	Field* fld;
	char* k;
	char* editrec;
	char* fldvalue;
	type_fields tf;
	bool changed;
	changed_rec* cr;
	changed_rec* cr2;
	int32_t i, j;
	TStream** ost;

	if(!edit) return;

	fld = fields[numfield];
	tf = fld->gettype();
	if(tf == type_fields::tf_version || tf == type_fields::tf_version8) return;
	if(null && !fld->getnull_exists()) return;

	fldvalue = new char[fld->getlen()];

	if(tf == type_fields::tf_string || tf == type_fields::tf_text || tf == type_fields::tf_image)
	{
		memset(fldvalue, 0, fld->getlen());
		k = fldvalue;
		if(fld->getnull_exists())
		{
			if(!null && st) *k = 1;
			k++;
		}
		*(TStream**)k = st;
	}
	else fld->get_binary_value(fldvalue, null, value);

	TableRecord *rec = nullptr;
	changed = true;
	if(phys_numrecord < phys_numrecords)
	{
		rec = getrecord(phys_numrecord);
		// TODO: Вменяемое сравнение в соответствии с типами
		changed = memcmp(rec->get_raw(fld), fldvalue, fld->len) != 0;
	}

	for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec == phys_numrecord) break;
	if(!cr)
	{
		if(!changed)
		{
			delete[] fldvalue;
			return; // значение не изменилось, ничего не делаем
		}
		cr = new changed_rec(this, phys_numrecord >= phys_numrecords ? changed_rec_type::inserted : changed_rec_type::changed, phys_numrecord);
		if(phys_numrecord <= phys_numrecords) {
			// TODO: тут должно быть что-то иное
			memcpy(cr->rec, rec, recordlen);
		}
	}

	editrec = cr->rec;

	if(cr->fields[numfield]) if(tf == type_fields::tf_string || tf == type_fields::tf_text || tf == type_fields::tf_image)
	{
		ost = (TStream**)(editrec + fld->offset + (fld->getnull_exists() ? 1 : 0));
		if(*ost != st)
		{
			delete *ost;
			*ost = nullptr;
		}
	}

	if(changed)
	{
		memcpy(editrec + fld->offset, fldvalue, fld->len);
		cr->fields[numfield] = 1;
	}
	else
	{
		cr->fields[numfield] = 0;
		j = 0;
		for(i = 0; i < num_fields; i++) j += cr->fields[i];
		if(j == 0)
		{
			// измененных полей больше нет, надо удалить запись из измененных
			if(ch_rec == cr) ch_rec = cr->next;
			else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
			{
				cr2->next = cr->next;
				break;
			}
			delete cr;
		}
		else memcpy(editrec + fld->offset, fldvalue, fld->len);
	}

	delete[] rec;
	delete[] fldvalue;
}

//---------------------------------------------------------------------------
void Table::restore_edit_value(uint32_t phys_numrecord, int32_t numfield)
{
	Field* fld;
	changed_rec* cr;
	changed_rec* cr2;
	int32_t i, j;
	type_fields tf;
	TStream** ost;

	if(phys_numrecord >= phys_numrecords) return;

	for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec == phys_numrecord) break;
	if(!cr) return;
	if(cr->changed_type != changed_rec_type::changed) return;

	fld = fields[numfield];
	tf = fld->gettype();
	if(cr->fields[numfield])
	{
		if(tf == type_fields::tf_string || tf == type_fields::tf_text || tf == type_fields::tf_image)
		{
			ost = (TStream**)(cr->rec + fld->offset + (fld->getnull_exists() ? 1 : 0));
			delete *ost;
			*ost = nullptr;
		}
	}

	cr->fields[numfield] = 0;
	j = 0;
	for(i = 0; i < num_fields; i++) j += cr->fields[i];
	if(j == 0)
	{
		// измененных полей больше нет, надо удалить запись из измененных
		if(ch_rec == cr) ch_rec = cr->next;
		else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
		{
			cr2->next = cr->next;
			break;
		}
		delete cr;
	}
	else{
		TableRecord *rec = getrecord(phys_numrecord);
		memcpy(cr->rec + fld->offset, rec->get_raw(fld), fld->len);
		delete rec;
	}

}

//---------------------------------------------------------------------------
void Table::set_rec_type(uint32_t phys_numrecord, changed_rec_type crt)
{
	changed_rec* cr;
	changed_rec* cr2;

	for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec == phys_numrecord) break;

	if(phys_numrecord < phys_numrecords)
	{
		switch(crt)
		{
			case changed_rec_type::changed:
				throw DetailedException("Попытка прямой установки признака \"Изменена\" существующей записи таблицы")
					.add_detail("Таблица", name)
					.add_detail("Физический номер записи", phys_numrecord);
				break;
			case changed_rec_type::inserted:
				throw DetailedException("Попытка прямой установки признака \"Добавлена\" существующей записи таблицы")
					.add_detail("Таблица", name)
					.add_detail("Физический номер записи", phys_numrecord);
				break;
			case changed_rec_type::deleted:
				if(cr)
				{
					cr->clear();
					delete[] cr->rec;
					delete[] cr->fields;
					cr->changed_type = crt;
				}
				else new changed_rec(this, crt, phys_numrecord);
				break;
			case changed_rec_type::not_changed:
				if(cr)
				{
					if(ch_rec == cr) ch_rec = cr->next;
					else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
					{
						cr2->next = cr->next;
						break;
					}
					delete cr;
				}
				break;
		}
	}
	else
	{
		switch(crt)
		{
			case changed_rec_type::changed:
				throw DetailedException("Попытка прямой установки признака \"Изменена\" добавленной записи таблицы")
					.add_detail("Таблица", name)
					.add_detail("Физический номер записи", phys_numrecord);
				break;
			case changed_rec_type::inserted:
				if(cr) cr->changed_type = crt;
				else
				{
					if(phys_numrecord > phys_numrecords + added_numrecords)
					{
						throw DetailedException("Попытка добавления записи таблицы, с номером больше максимального")
							.add_detail("Таблица", name)
							.add_detail("Максимальный номер записи", phys_numrecords + added_numrecords)
							.add_detail("Физический номер записи", phys_numrecord);
					}
					else
					{
						new changed_rec(this, crt, phys_numrecord);
						added_numrecords++;
					}
				}
				break;
			case changed_rec_type::deleted:
				if(cr)
				{
					if(ch_rec == cr) ch_rec = cr->next;
					else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
					{
						cr2->next = cr->next;
						break;
					}
					delete cr;
					added_numrecords--;
				}
				for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec > phys_numrecord) cr->numrec--;
				break;
			case changed_rec_type::not_changed:
				throw DetailedException("Попытка прямой установки признака \"Не изменена\" добавленной записи таблицы")
					.add_detail("Таблица", name)
					.add_detail("Физический номер записи", phys_numrecord);
				break;
		}
	}
}

//---------------------------------------------------------------------------
TableRecord *Table::get_edit_record(uint32_t phys_numrecord)
{
	changed_rec* cr;
	for(cr = ch_rec; cr; cr = cr->next) if(phys_numrecord == cr->numrec)
	{
		if(cr->changed_type != changed_rec_type::deleted)
		{
			char *rec = new char[recordlen];
			memcpy(rec, cr->rec, recordlen);
			return new TableRecord(this, rec, recordlen);
		}
		break;
	}
	return getrecord(phys_numrecord);
}

//---------------------------------------------------------------------------
uint32_t Table::get_phys_numrec(int32_t ARow, Index* cur_index)
{
	uint32_t numrec;

	if(ARow == 0)
	{
		throw DetailedException("Попытка получения номера физической записи по нулевому номеру строки.")
			.add_detail("Таблица", name);
	}

	if(edit)
	{
		if((uint32_t)ARow > log_numrecords + added_numrecords)
		{
			throw DetailedException("Попытка получения номера физической записи по номеру строки, превышающему количество записей")
				.add_detail("Таблица", name)
				.add_detail("Количество логических записей", log_numrecords)
				.add_detail("Количество добавленных записей", added_numrecords)
				.add_detail("Номер строки", ARow);
		}
		if((uint32_t)ARow > log_numrecords) return ARow - 1 - log_numrecords + phys_numrecords;
	}

	if((uint32_t)ARow > log_numrecords)
	{
		throw DetailedException("Попытка получения номера физической записи по номеру строки, превышающему количество записей")
			.add_detail("Таблица", name)
			.add_detail("Количество логических записей", log_numrecords)
			.add_detail("Номер строки", ARow);
	}
	if(cur_index) numrec = cur_index->get_numrec(ARow - 1);
	else
	{
		tr_syn->BeginRead();
		numrec = recordsindex[ARow - 1];
		tr_syn->EndRead();
	}

	return numrec;
}

//---------------------------------------------------------------------------
void Table::create_file_data()
{
	if(!file_data) return;
	file_data = new v8object(base);
	refresh_descr_table();
}

//---------------------------------------------------------------------------
void Table::create_file_blob()
{
	if(!file_blob) return;
	file_blob = new v8object(base);
	refresh_descr_table();
}

//---------------------------------------------------------------------------
void Table::create_file_index()
{
	if(!file_index) return;
	file_index = new v8object(base);
	refresh_descr_table();

}

//---------------------------------------------------------------------------
void Table::refresh_descr_table()
{
	// TODO: Действительно ли тут ошибка?? и с такой ли формулировкой??
	throw DetailedException("Попытка обновления файла описания таблицы.")
		.add_detail("Таблица", name);
	return;

}

//---------------------------------------------------------------------------
void Table::delete_data_record(uint32_t phys_numrecord)
{
	int32_t first_empty_rec;

	if(!edit)
	{
		throw DetailedException("Попытка удаления записи из файла file_data не в режиме редактирования.")
			.add_detail("Таблица", name)
			.add_detail("Индекс удаляемой записи", phys_numrecord);
	}

	if(!file_data)
	{
		throw DetailedException("Попытка удаления записи из несуществующего файла file_data.")
			.add_detail("Таблица", name)
			.add_detail("Индекс удаляемой записи", phys_numrecord);
		return;
	}

	if(phys_numrecord >= phys_numrecords)
	{
		throw DetailedException("Попытка удаления записи в файле file_data за пределами файла.")
			.add_detail("Таблица", name)
			.add_detail("Всего записей", phys_numrecords)
			.add_detail("Индекс удаляемой записи", phys_numrecord);
	}

	if(!phys_numrecord)
	{
		throw DetailedException("Попытка удаления нулевой записи в файле file_data.")
			.add_detail("Таблица", name)
			.add_detail("Всего записей", phys_numrecords);
	}

	if(phys_numrecord == phys_numrecords - 1)
	{
		file_data->set_len(phys_numrecord * recordlen);
		phys_numrecords--;
	}
	else
	{
		char *tmp = new char[recordlen];
		memset(tmp, 0, recordlen);
		TableRecord *rec = new TableRecord(this, tmp, recordlen);

		file_data->getdata(&first_empty_rec, 0, 4);
		*((int32_t*)rec) = first_empty_rec;
		file_data->setdata(&first_empty_rec, 0, 4);
		write_data_record(phys_numrecord, rec);
		delete rec;
	}

}

//---------------------------------------------------------------------------
void Table::delete_blob_record(uint32_t blob_numrecord)
{
	int32_t prev_free_first;
	int32_t i, j;

	if(!edit)
	{
		throw DetailedException("Попытка удаления записи из файла file_blob не в режиме редактирования.")
			.add_detail("Таблица", name)
			.add_detail("Смещение удаляемой записи", blob_numrecord << 8);
	}

	if(!file_blob)
	{
		throw DetailedException("Попытка удаления записи из несуществующего файла file_blob.")
			.add_detail("Таблица", name)
			.add_detail("Смещение удаляемой записи", blob_numrecord << 8);
	}

	if(blob_numrecord << 8 >= file_blob->getlen())
	{
		throw DetailedException("Попытка удаления записи в файле file_blob за пределами файла.")
			.add_detail("Таблица", name)
			.add_detail("Смещение удаляемой записи", blob_numrecord << 8)
			.add_detail("Длина файла", file_blob->getlen());
	}

	if(blob_numrecord == 0)
	{
		throw DetailedException("Попытка удаления нулевой записи в файле file_blob.")
			.add_detail("Таблица", name)
			.add_detail("Длина файла", file_blob->getlen());
	}

	file_blob->getdata(&prev_free_first, 0, 4); // читаем предыдущее начало свободных блоков
	for(i = blob_numrecord; i; file_blob->getdata(&i, i << 8, 4)) j = i; // ищем последний блок в цепочке удаляемых
	file_blob->setdata(&prev_free_first, j << 8, 4); // устанавливаем в конец цепочки удаляемых блоков старое начало цепочки свободных блоков
	file_blob->setdata(&blob_numrecord, 0, 4); // устанавливаем новое начало цепочки свободных блоков на начало удаляемых блоков
}

//---------------------------------------------------------------------------
void Table::delete_index_record(uint32_t phys_numrecord)
{
	TableRecord *rec = getrecord(phys_numrecord);
	delete_index_record(phys_numrecord, rec);
	delete rec;
}

//---------------------------------------------------------------------------
void Table::delete_index_record(uint32_t phys_numrecord, const TableRecord *rec)
{
	if(rec->is_removed()) {
		throw DetailedException("Попытка удаления индекса удаленной записи.")
			.add_detail("Таблица", name)
			.add_detail("Физический номер записи", phys_numrecord);
	}

	for(int32_t i = 0; i < num_indexes; i++) indexes[i]->delete_index(rec, phys_numrecord);
}

//---------------------------------------------------------------------------
void Table::write_data_record(uint32_t phys_numrecord, const TableRecord *rec)
{
	char* b;

	if(!edit)
	{
		throw DetailedException("Попытка записи в файл file_data не в режиме редактирования.")
			.add_detail("Таблица", name)
			.add_detail("Индекс записываемой записи", phys_numrecord);
	}

	if(!file_data) create_file_data();

	if(phys_numrecord > phys_numrecords && !(phys_numrecord == 1 && phys_numrecords == 0))
	{
		throw DetailedException("Попытка записи в файл file_data за пределами файла.")
			.add_detail("Таблица", name)
			.add_detail("Всего записей", phys_numrecords)
			.add_detail("Индекс записываемой записи", phys_numrecord);
	}

	if(!phys_numrecord)
	{
		throw DetailedException("Попытка записи нулевой записи в файл file_data.")
			.add_detail("Таблица", name)
			.add_detail("Всего записей", phys_numrecords);
	}

	if(phys_numrecords == 0)
	{
		b = new char[recordlen];
		memset(b, 0, recordlen);
		b[0] = 1;
		file_data->setdata(b, 0, recordlen);
		delete[] b;
	}
	file_data->setdata(rec, phys_numrecord * recordlen, recordlen);
}

//---------------------------------------------------------------------------
uint32_t Table::write_blob_record(char* blob_record, uint32_t blob_len)
{
	uint32_t cur_block, cur_offset, prev_offset, first_block, next_block;
	uint16_t cur_len;
	uint32_t zero = 0;

	if(!edit)
	{
		throw DetailedException("Попытка записи в файл file_blob не в режиме редактирования.")
			.add_detail("Таблица", name);
	}

	if(!blob_len) return 0;

	if(!file_blob)
	{
		create_file_blob();
		char* b = new char[BLOB_RECORD_LEN];
		memset(b, 0, BLOB_RECORD_LEN);
		file_blob->setdata(b, 0, BLOB_RECORD_LEN);
		delete[] b;
	}

	file_blob->getdata(&first_block, 0, 4);
	if(!first_block) first_block = file_blob->getlen() >> 8;
	prev_offset = 0;

	for(cur_block = first_block; blob_len; blob_len -= cur_len, cur_block = next_block, blob_record += cur_len)
	{
		cur_len = std::min(blob_len, BLOB_RECORD_DATA_LEN);
		if(cur_len < BLOB_RECORD_DATA_LEN) memset(blob_record, 0, BLOB_RECORD_DATA_LEN);

		if(prev_offset) file_blob->setdata(&cur_block, prev_offset, 4);

		cur_offset = cur_block << 8;
		next_block = 0;
		if(cur_offset < file_blob->getlen()) file_blob->getdata(&next_block, cur_offset, 4);

		file_blob->setdata(&zero, cur_offset, 4);
		file_blob->setdata(&cur_len, cur_offset + 4, 2);
		file_blob->setdata(blob_record, cur_offset + 6, BLOB_RECORD_DATA_LEN);

		if(!next_block) next_block = file_blob->getlen() >> 8;
		prev_offset = cur_offset;
	}
	if(next_block << 8 < file_blob->getlen()) file_blob->setdata(&next_block, 0, 4);
	else file_blob->setdata(&zero, 0, 4);

	return first_block;
}

//---------------------------------------------------------------------------
uint32_t Table::write_blob_record(TStream* bstr)
{
	uint32_t cur_block, cur_offset, prev_offset, first_block, next_block;
	uint16_t cur_len;
	uint32_t blob_len;
	uint32_t zero;
	char blob_record[BLOB_RECORD_LEN];

	if(!edit)
	{
		throw DetailedException("Попытка записи в файл file_blob не в режиме редактирования.")
			.add_detail("Таблица", name);
	}

	blob_len = bstr->GetSize();
	if(!blob_len) return 0;
	bstr->Seek(0, soFromBeginning);
	zero = 0;

	if(!file_blob)
	{
		create_file_blob();
		memset(blob_record, 0, BLOB_RECORD_LEN);
		file_blob->setdata(blob_record, 0, BLOB_RECORD_LEN);
	}

	file_blob->getdata(&first_block, 0, 4);
	if(!first_block) first_block = file_blob->getlen() >> 8;
	prev_offset = 0;

	for(cur_block = first_block; blob_len; blob_len -= cur_len, cur_block = next_block)
	{
		cur_len = std::min(blob_len, BLOB_RECORD_DATA_LEN);
		if(cur_len < BLOB_RECORD_DATA_LEN) memset(blob_record, 0, BLOB_RECORD_DATA_LEN); //-V512
		bstr->Read(blob_record, cur_len);

		if(prev_offset) file_blob->setdata(&cur_block, prev_offset, 4);

		cur_offset = cur_block << 8;
		next_block = 0;
		if(cur_offset < file_blob->getlen()) file_blob->getdata(&next_block, cur_offset, 4);

		file_blob->setdata(&zero, cur_offset, 4);
		file_blob->setdata(&cur_len, cur_offset + 4, 2);
		file_blob->setdata(blob_record, cur_offset + 6, BLOB_RECORD_DATA_LEN);

		if(!next_block) next_block = file_blob->getlen() >> 8;
		prev_offset = cur_offset;
	}
	if(next_block << 8 < file_blob->getlen()) file_blob->setdata(&next_block, 0, 4);
	else file_blob->setdata(&zero, 0, 4);

	return first_block;
}

//---------------------------------------------------------------------------

void Table::write_index_record(const uint32_t phys_numrecord, const TableRecord *rec)
{
	if(rec->is_removed()) {
		throw DetailedException("Попытка записи индексов помеченной на удаление записи.")
			.add_detail("Таблица", name);
	}

	for(int32_t i = 0; i < num_indexes; i++) indexes[i]->write_index(phys_numrecord, rec);

}

//---------------------------------------------------------------------------
void Table::cancel_edit()
{
	changed_rec* cr;
	changed_rec* cr2;
	for(cr = ch_rec; cr;)
	{
		cr2 = cr->next;
		delete cr;
		cr = cr2;
	}

	edit = false;
	ch_rec = nullptr;
	added_numrecords = 0;
}

//---------------------------------------------------------------------------
void Table::end_edit()
{
	changed_rec* cr;

	// удаляем удаленные записи
	for (cr = ch_rec; cr; cr = cr->next) {
		if (cr->changed_type == changed_rec_type::deleted) {
			delete_record(cr->numrec);
		}
	}

	// записываем измененные записи
	for (cr = ch_rec; cr; cr = cr->next) {
		if (cr->changed_type == changed_rec_type::changed) {
			update_record(cr->numrec, cr->rec, cr->fields);
		}
	}

	// добавляем новые записи
	for (cr = ch_rec; cr; cr = cr->next) {
		if (cr->changed_type == changed_rec_type::inserted) {
			insert_record(new TableRecord(this, cr->rec));
		}
	}

	cancel_edit();
	base->flush();
}

//---------------------------------------------------------------------------
void Table::delete_record(uint32_t phys_numrecord)
{
	int32_t i;
	type_fields tf;
	TableRecord *rec = getrecord(phys_numrecord);

	delete_index_record(phys_numrecord, rec);

	for(i = 0; i < num_fields; i++)
	{
		Field *f = fields[i];
		tf = f->type_manager->gettype();
		if(tf == type_fields::tf_image || tf == type_fields::tf_string || tf == type_fields::tf_text)
		{
			auto bp = (const table_blob_file *)rec->get_raw(f);
			if (bp->blob_start) {
				delete_blob_record(bp->blob_start);
			}
		}
	}

	delete_data_record(phys_numrecord);

	delete rec;
}

//---------------------------------------------------------------------------
void Table::insert_record(const TableRecord *nrec)
{
	int32_t offset;
	char* j;
	type_fields tf;
	uint32_t phys_numrecord;
	uint32_t k;

	TableRecord *rec = new TableRecord(this);
	rec->Assign(nrec);

	if(!file_data) create_file_data();

	for (int i = 0; i < num_fields; i++)
	{
		Field *f = fields[i];
		tf = f->type_manager->gettype();
		offset = f->offset + (f->getnull_exists() ? 1 : 0);
		switch(tf)
		{
			case type_fields::tf_image:
			case type_fields::tf_string:
			case type_fields::tf_text: {
				TStream **st = (TStream **) (rec + offset);
				table_blob_file bp = {0, 0};
				if (*st) {
					bp.blob_length = (*st)->GetSize();
					bp.blob_start = write_blob_record(*st);
					delete *st;
					*st = nullptr;
				}
				if (bp.blob_start == 0 && f->getnull_exists()) {
					rec->set_null(f);
				} else {
					rec->set_data(f, &bp);
				}
			}
			case type_fields::tf_version: {

				struct {
					_version ver;
					_version ver2;
				} vers;

				file_data->get_version_rec_and_increase(&vers.ver);
				vers.ver2 = vers.ver;
				rec->set_data(f, &vers);

				break;
			}
			case type_fields::tf_version8: {
				_version ver;
				file_data->get_version_rec_and_increase(&ver);
				rec->set_data(f, &ver);
				break;
			}
		}
	}

	if(file_data->len == 0)
	{
		j = new char[recordlen];
		memset(j, 0, recordlen);
		*j = 1;
		file_data->setdata(j, 0, recordlen);
		delete[] j;
		phys_numrecord = 1;
	}
	else
	{
		file_data->getdata(&phys_numrecord, 1, 4);
		if(phys_numrecord)
		{
			file_data->getdata(&k, phys_numrecord * recordlen + 1, 4);
			file_data->setdata(&k, 1, 4);
		}
		else phys_numrecord = file_data->len / recordlen;
	}

	write_data_record(phys_numrecord, rec);
	write_index_record(phys_numrecord, rec);

}

//---------------------------------------------------------------------------
void Table::update_record(uint32_t phys_numrecord, char* newdata, char* changed_fields)
{
	int32_t i, offset;
	type_fields tf;
	_version ver;
	TStream** st;

	TableRecord *rec = new TableRecord(this, newdata, recordlen);
	TableRecord *orec = getrecord(phys_numrecord);
	delete_index_record(phys_numrecord, orec);
	for(i = 0; i < num_fields; i++)
	{
		table_blob_file new_blob = {0, 0};
		Field *f = fields[i];
		tf = f->type_manager->gettype();
		offset = f->offset + (f->getnull_exists() ? 1 : 0);
		if(changed_fields[i])
		{
			if(tf == type_fields::tf_image || tf == type_fields::tf_string || tf == type_fields::tf_text)
			{
				if(f->getnull_exists())
				{
					if (orec->is_null_value(f))
					{
						auto bp = orec->get<table_blob_file>(f);
						if (bp.blob_start != 0) {
							delete_blob_record(bp.blob_start);
						}
					}
					if (!rec->is_null_value(f))
					{
						st = (TStream**)(rec->get_data(f)); // TODO: не забыть про сей костыль
						if(*st)
						{
							new_blob.blob_length = (*st)->GetSize();
							new_blob.blob_start = write_blob_record(*st);
							delete *st;
							*st = nullptr;
						}
					}
				}
				else
				{
					auto old_blob = orec->get<table_blob_file>(f);
					if (old_blob.blob_start != 0) {
						delete_blob_record(old_blob.blob_start);
					}

					st = (TStream**)rec->get_data(f); // TODO: не забыть про сей костыль
					if(*st)
					{
						new_blob.blob_length = (*st)->GetSize();
						new_blob.blob_start = write_blob_record(*st);
						delete *st;
						*st = nullptr;
					}
				}
				orec->set_data(f, &new_blob);
				if (new_blob.blob_start == 0) {
					if (f->getnull_exists()) {
						orec->set_null(f);
					}
				}
			}
			else memcpy(orec + f->offset, rec + f->offset, f->len);
		}
		else
		{
			if(tf == type_fields::tf_version)
			{
				file_data->get_version_rec_and_increase(&ver);
				memcpy(orec + offset + 8, &ver, 8);
			}
			else if(tf == type_fields::tf_version8)
			{
				file_data->get_version_rec_and_increase(&ver);
				memcpy(orec + offset, &ver, 8);
			}
		}

	}

	write_index_record(phys_numrecord, orec);
	write_data_record(phys_numrecord, orec);
	delete[] orec;

}

//---------------------------------------------------------------------------
// получить шаблон проверки записи (массив, содержащий для каждого байта массив из 256 байт, содержащий 0, если значение не допустимо и 1, если допустимо)
char* Table::get_record_template_test()
{
	int32_t len;
	char* res;
	char* curp;
	int32_t i, j, l;
	Field* f;
	bool required;

	len = recordlen << 8;
	res = new char[len];
	memset(res, 0, len);

	for(i = 0; i < num_fields; i++)
	{
		required = false;
		f = fields[i];
		curp = res + (f->getoffset() << 8);

		if(f->getnull_exists())
		{
			curp[0] = 1;
			curp[1] = 1;
			curp += BLOB_RECORD_LEN;
		}

		l = f->getlength();
		switch(f->gettype())
		{
			case type_fields::tf_binary: // B // длина = length
				memset(curp, 1, BLOB_RECORD_LEN * l);
				break;
			case type_fields::tf_bool: // L // длина = 1
				curp[0] = 1;
				curp[1] = 1;
				break;
			case type_fields::tf_numeric: // N // длина = (length + 2) / 2
				j = (l + 2) / 2;
				for(; j > 0; --j)
				{
					memcpy(curp, NUM_TEST_TEMPLATE, BLOB_RECORD_LEN);
					curp += BLOB_RECORD_LEN;
				}
				break;
			case type_fields::tf_char: // NC // длина = length * 2
				memset(curp, 1, BLOB_RECORD_LEN * 2 * l);
				break;
			case type_fields::tf_varchar: // NVC // длина = length * 2 + 2
				if(l > 255) j = BLOB_RECORD_LEN;
				else j = l + 1;
				memset(curp, 1, j);
				//curp[0x20] = 1;
				curp += BLOB_RECORD_LEN;
				j = (l >> 8) + 1;
				memset(curp, 1, j);
				curp += BLOB_RECORD_LEN;
				memset(curp, 1, BLOB_RECORD_LEN * 2 * l);
				break;
			case type_fields::tf_version: // RV // 16, 8 версия создания и 8 версия модификации ? каждая версия int32_t(изменения) + int32_t(реструктуризация)
				memset(curp, 1, BLOB_RECORD_LEN * 16);
				break;
			case type_fields::tf_string: // NT // 8 (unicode text)
				memset(curp, 1, BLOB_RECORD_LEN * 8);
				break;
			case type_fields::tf_text: // T // 8 (ascii text)
				memset(curp, 1, BLOB_RECORD_LEN * 8);
				break;
			case type_fields::tf_image: // I // 8 (image = bynary data)
				memset(curp, 1, BLOB_RECORD_LEN * 8);
				break;
			case type_fields::tf_datetime: // DT //7
				if (EqualIC(f->getname(), "_DATE_TIME")) required = true;
				else if (EqualIC(f->getname(), "_NUMBERPREFIX")) required = true;

				memcpy(curp, DATE1_TEST_TEMPLATE, BLOB_RECORD_LEN);
				curp += BLOB_RECORD_LEN;
				memcpy(curp, NUM_TEST_TEMPLATE, BLOB_RECORD_LEN);
				curp += BLOB_RECORD_LEN;
				memcpy(curp, DATE3_TEST_TEMPLATE, BLOB_RECORD_LEN);
				if(required) curp[0] = 0;
				curp += BLOB_RECORD_LEN;
				memcpy(curp, DATE4_TEST_TEMPLATE, BLOB_RECORD_LEN);
				if(required) curp[0] = 0;
				curp += BLOB_RECORD_LEN;
				memcpy(curp, DATE5_TEST_TEMPLATE, BLOB_RECORD_LEN);
				curp += BLOB_RECORD_LEN;
				memcpy(curp, DATE67_TEST_TEMPLATE, BLOB_RECORD_LEN);
				curp += BLOB_RECORD_LEN;
				memcpy(curp, DATE67_TEST_TEMPLATE, BLOB_RECORD_LEN);
				break;
			case type_fields::tf_version8: // 8, скрытое поле при recordlock == false и отсутствии поля типа tf_version
				memset(curp, 1, BLOB_RECORD_LEN * 8);
				break;
			case type_fields::tf_varbinary: // VB // длина = length + 2
				if(l > 255) j = BLOB_RECORD_LEN;
				else j = l + 1;
				memset(curp, 1, j);
				curp += BLOB_RECORD_LEN;
				j = (l >> 8) + 1;
				memset(curp, 1, j);
				curp += BLOB_RECORD_LEN;
				memset(curp, 1, BLOB_RECORD_LEN * l);
				break;
		}
	}

	res[0] = 1;

	return res;
}

//---------------------------------------------------------------------------
// заполнить recordsindex не динамически
void Table::fillrecordsindex()
{
	if (recordsindex_complete) {
		return;
	}
	recordsindex.clear();

	for (uint32_t i = 0; i < phys_numrecords; i++) {
		std::shared_ptr<TableRecord> rec(getrecord(i));
		if (rec->is_removed()) {
			continue;
		}
		recordsindex.push_back(i);
	}
	recordsindex_complete = true;
	numrecords_review = phys_numrecords;
	numrecords_found = recordsindex.size();
	log_numrecords = recordsindex.size();
}

std::string Table::get_file_name_for_field(int32_t num_field, char *rec, uint32_t numrec)
{
	std::string s("");
	int32_t i;
	Index* ind;

	if(num_indexes)
	{
		ind = indexes[0];
		for(i = 0; i < num_indexes; i++) if(indexes[i]->is_primary)
		{
			ind = indexes[i];
			break;
		}
		for(i = 0; i < ind->num_records; i++)
		{
			if(!s.empty()) {
				s += "_";
			}
			s += ind->records[i].field->get_XML_presentation(rec);
		}
		if(!ind->is_primary && numrec){
			s += "_";
			s += numrec;
		}
	}
	if(!issystem || !(EqualIC(name, "CONFIG")
					  || EqualIC(name, "CONFIGSAVE")
					  || EqualIC(name, "FILES")
					  || EqualIC(name, "PARAMS")))
	{
		if (!s.empty()) {
			s += "_";
		}
		s += fields[num_field]->getname();
	}

	return s;
}

std::string Table::get_file_name_for_record(const TableRecord *rec)
{
	std::string s("");

	int32_t i;
	int32_t num_rec;

	Index* ind;

	if(num_indexes)
	{
		ind = indexes[0];
		for(i = 0; i < num_indexes; i++)
		{

			if(indexes[i]->is_primary)
			{
				ind = indexes[i];
				break;
			}
		}
		num_rec = ind->num_records;

		for(i = 0; i < num_rec; i++)
		{
			if (!s.empty()){
				s += "_";
			}
			Field* tmp_field = ind->records[i].field;
			std::string tmp_str = rec->get_string(tmp_field);

			s += tmp_str;

		}
	}

	return s;
}

Field* Table::get_field(const std::string &fieldname) const
{
	Field* fld = find_field(fieldname);
	if (fld) {
		return fld;
	}
	DetailedException error("Поле не найдено!");
	error.add_detail("Имя поля", fieldname);
	error.add_detail("Таблица", name);
	throw error;
}

Field* Table::find_field(const std::string &fieldname) const throw()
{
	for (int32_t j = 0; j < num_fields; j++) {
		Field* fld = fields[j];
		if (EqualIC(fld->getname(), fieldname)) {
			return fld;
		}
	}

	return nullptr;
}

Index* Table::get_index(const std::string &indexname) const
{
	Index* ind = find_index(indexname);
	if (ind) {
		return ind;
	}

	DetailedException error("Индекс не найден!");
	error.add_detail("Имя индекса", indexname);
	error.add_detail("Таблица", name);
	throw error;
}

Index* Table::find_index(const std::string &indexname) const throw()
{
	for (int32_t j = 0; j < num_indexes; j++) {
		Index* ind = indexes[j];
		if (EqualIC(ind->getname(), indexname)) {
			return ind;
		}
	}

	return nullptr;
}
