/*
 *
 *
 *
 *
 */

#include "TableFiles.h"
#include "Common.h"

extern Registrator msreg_g;

//********************************************************
// Класс TableFile

//---------------------------------------------------------------------------
TableFile::TableFile(Table* _t, const String& _name, uint32_t _maxpartno)
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
	String s;
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

	for (int i = 0; i < table->get_phys_numrecords(); ++i) {

		std::shared_ptr<TableRecord> record (table->getrecord(i));
		if(record->is_removed()) {
			continue;
		}
		auto b = (const table_blob_file *)record->get_data(fld_blob_pointer);
		if (b->blob_start == 0 || b->blob_length == 0) {
			continue;
		}

		tr.name = record->get_string(filename);
		if(tr.name.IsEmpty()) continue;

		tr.addr.blob_start = b->blob_start;
		tr.addr.blob_length = b->blob_length;

		if(partno) tr.partno = record->get_string(partno).ToIntDef(0);
		else tr.partno = 0;
		time1CD_to_FileTime(&tr.ft_create, record->get_data(fld_create));
		time1CD_to_FileTime(&tr.ft_modify, record->get_data(fld_modify));

		allrec.push_back(tr);

		s = tr.name.UpperCase();
		pmaxpartno = maxpartnos.find(s);
		if(pmaxpartno == maxpartnos.end()) maxpartnos[s] = tr.partno;
		else if(pmaxpartno->second < tr.partno) pmaxpartno->second = tr.partno;
	}

	for (auto pmaxpartno : maxpartnos) {
		tf = new TableFile(table, pmaxpartno.first, pmaxpartno.second);
		allfiles[pmaxpartno.first] = tf;
	}

	for (int j = 0; j < allrec.size(); ++j) {
		ptr = &(allrec[j]);
		pfilesmap = allfiles.find(ptr->name.UpperCase());
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
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. В таблице меньше 6 полей"
			,"Таблица", table->getname()
			,"Кол-во полей", table->get_numfields());
		return false;
	}

	if(table->get_numfields() > 7)
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. В таблице больше 7 полей"
			,"Таблица", table->getname()
			,"Кол-во полей", table->get_numfields());
		return false;
	}

	if(table->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Первое поле таблицы не FILENAME"
			,"Таблица", table->getname()
			,"Поле", table->getfield(0)->getname());
		return false;
	}

	if(table->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Второе поле таблицы не CREATION"
			,"Таблица", table->getname()
			,"Поле", table->getfield(1)->getname());
		return false;
	}

	if(table->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Третье поле таблицы не MODIFIED"
			,"Таблица", table->getname()
			,"Поле", table->getfield(2)->getname());
		return false;
	}

	if(table->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Четвертое поле таблицы не ATTRIBUTES"
			,"Таблица", table->getname()
			,"Поле", table->getfield(3)->getname());
		return false;
	}

	if(table->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Пятое поле таблицы не DATASIZE"
			,"Таблица", table->getname()
			,"Поле", table->getfield(4)->getname());
		return false;
	}

	if(table->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Шестое поле таблицы не BINARYDATA"
			,"Таблица", table->getname()
			,"Поле", table->getfield(5)->getname());
		return false;
	}

	if(table->get_numfields() > 6)
	{
		if(table->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Седьмое поле таблицы не PARTNO"
				,"Таблица", table->getname()
				,"Поле", table->getfield(6)->getname());
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
TableFile* TableFiles::getfile(const String& name)
{
	std::map<String,TableFile*>::iterator p;

	p = allfiles.find(name.UpperCase());
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
