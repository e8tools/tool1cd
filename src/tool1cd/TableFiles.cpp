/*
 *
 *
 *
 *
 */

#include "TableFiles.h"
#include "Common.h"
#include "DetailedException.h"

using namespace std;

extern Registrator msreg_g;

//********************************************************
// Класс TableFile

//---------------------------------------------------------------------------
TableFile::TableFile(Table *_t, const string &_name, uint32_t _maxpartno)
{
	uint32_t i;

	t = _t;
	name = _name;
	maxpartno = _maxpartno;
	addr = new table_blob_file[maxpartno + 1];
	for(i = 0; i <= maxpartno; ++i)
	{
		addr[i].blob_start = 0;
		addr[i].blob_length = 0;
	}
	memset(&ft_create, 0, sizeof(ft_create));
	memset(&ft_modify, 0, sizeof(ft_modify));
}

//---------------------------------------------------------------------------
TableFile::~TableFile()
{
	delete[] addr;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс TableFiles

//---------------------------------------------------------------------------
TableFiles::TableFiles(Table* t)
{
	Field* partno;
	table_rec tr, *ptr;
	std::vector<table_rec> allrec;
	std::map<String,int32_t> maxpartnos;
	std::map<String,int32_t>::iterator pmaxpartno;
	TableFile* tf;
	std::map<String,TableFile*>::iterator pfilesmap;

	table = t;
	ready = test_table();
	if(!ready) return;

	Field *filename = table->getfield(0);

	Field *fld_create = table->getfield(1);
	Field *fld_modify = table->getfield(2);
	Field *fld_blob_pointer = table->getfield(5);

	if(table->get_numfields() > 6) partno = table->getfield(6);
	else partno = nullptr;

	for (uint32_t i = 0; i < table->get_phys_numrecords(); ++i) {

		std::shared_ptr<TableRecord> record (table->getrecord(i));
		if(record->is_removed()) {
			continue;
		}
		auto b = record->get<table_blob_file>(fld_blob_pointer);
		if (b.blob_start == 0 || b.blob_length == 0) {
			continue;
		}

		tr.name = record->get_string(filename);
		if (tr.name.empty()) {
			continue;
		}

		tr.addr = b;

		if(partno) tr.partno = ToIntDef(record->get_string(partno), 0);
		else tr.partno = 0;
		time1CD_to_FileTime(&tr.ft_create, record->get_data(fld_create));
		time1CD_to_FileTime(&tr.ft_modify, record->get_data(fld_modify));

		allrec.push_back(tr);

		string s = LowerCase(tr.name);
		pmaxpartno = maxpartnos.find(s);
		if(pmaxpartno == maxpartnos.end()) maxpartnos[s] = tr.partno;
		else if(pmaxpartno->second < tr.partno) pmaxpartno->second = tr.partno;
	}

	for (auto pmaxpartno : maxpartnos) {
		tf = new TableFile(table, pmaxpartno.first, pmaxpartno.second);
		allfiles[pmaxpartno.first] = tf;
	}

	for (size_t j = 0; j < allrec.size(); ++j) {
		ptr = &(allrec[j]);
		pfilesmap = allfiles.find(LowerCase(ptr->name));
		tf = pfilesmap->second;
		tf->addr[ptr->partno] = ptr->addr;
		if(!ptr->partno)
		{
			tf->ft_create = ptr->ft_create;
			tf->ft_modify = ptr->ft_modify;
			tf->name = ptr->name;
		}
	}
}

//---------------------------------------------------------------------------
TableFiles::~TableFiles()
{
	for (auto p : allfiles) {
		delete p.second;
	}
	allfiles.clear();
}

//---------------------------------------------------------------------------
bool TableFiles::test_table()
{
	if(!table) return false;
	if(table->get_numfields() < 6)
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. В таблице меньше 6 полей")
			.add_detail("Таблица", table->getname())
			.add_detail("Кол-во полей", table->get_numfields());
	}

	if(table->get_numfields() > 7)
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. В таблице больше 7 полей")
			.add_detail("Таблица", table->getname())
			.add_detail("Кол-во полей", table->get_numfields());
	}

	if (CompareIC(table->getfield(0)->getname(), "FILENAME"))
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. Первое поле таблицы не FILENAME")
			.add_detail("Таблица", table->getname())
			.add_detail("Поле", table->getfield(0)->getname());
	}

	if (CompareIC(table->getfield(1)->getname(), "CREATION"))
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. Второе поле таблицы не CREATION")
			.add_detail("Таблица", table->getname())
			.add_detail("Поле", table->getfield(1)->getname());
	}

	if (CompareIC(table->getfield(2)->getname(), "MODIFIED"))
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. Третье поле таблицы не MODIFIED")
			.add_detail("Таблица", table->getname())
			.add_detail("Поле", table->getfield(2)->getname());
	}

	if (CompareIC(table->getfield(3)->getname(), "ATTRIBUTES"))
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. Четвертое поле таблицы не ATTRIBUTES")
			.add_detail("Таблица", table->getname())
			.add_detail("Поле", table->getfield(3)->getname());
	}

	if (CompareIC(table->getfield(4)->getname(), "DATASIZE"))
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. Пятое поле таблицы не DATASIZE")
			.add_detail("Таблица", table->getname())
			.add_detail("Поле", table->getfield(4)->getname());
	}

	if (CompareIC(table->getfield(5)->getname(), "BINARYDATA"))
	{
		throw DetailedException("Ошибка проверки таблицы контейнера файлов. Шестое поле таблицы не BINARYDATA")
			.add_detail("Таблица", table->getname())
			.add_detail("Поле", table->getfield(5)->getname());
	}

	if(table->get_numfields() > 6)
	{
		if (CompareIC(table->getfield(6)->getname(), "PARTNO"))
		{
			throw DetailedException("Ошибка проверки таблицы контейнера файлов. Седьмое поле таблицы не PARTNO")
				.add_detail("Таблица", table->getname())
				.add_detail("Поле", table->getfield(6)->getname());
		}
	}
	return true;
}

//---------------------------------------------------------------------------
TableFile* TableFiles::getfile(const string &name)
{
	std::map<String,TableFile*>::iterator p;

	p = allfiles.find(LowerCase(name));
	if(p == allfiles.end()) {
		return nullptr;
	}
	else {
		return p->second;
	}
}

std::map<String,TableFile*> &TableFiles::files()
{
	return allfiles;
}
