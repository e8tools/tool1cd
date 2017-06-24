/*
 *
 *
 *
 *
 */

#include "TableFiles.h"

extern Registrator msreg_g;

//********************************************************
// Класс table_file

//---------------------------------------------------------------------------
table_file::table_file(Table* _t, const String& _name, uint32_t _maxpartno)
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
table_file::~table_file()
{
	delete[] addr;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс TableFiles

//---------------------------------------------------------------------------
TableFiles::TableFiles(Table* t)
{
	Field* filename;
	Field* f;
	Field* partno;
	int32_t* start;
	int32_t* length;
	unsigned char* create;
	unsigned char* modify;
	uint32_t i;
	size_t j;
	String s;
	table_rec tr, *ptr;
	std::vector<table_rec> allrec;
	std::map<String,int32_t> maxpartnos;
	std::map<String,int32_t>::iterator pmaxpartno;
	table_file* tf;
	std::map<String,table_file*>::iterator pfilesmap;

	tab = t;
	ready = test_table();
	if(!ready) return;

	rec = new char[tab->get_recordlen()];

	filename = tab->getfield(0);

	f = tab->getfield(1);
	create = (unsigned char*)(rec + f->getoffset());
	f = tab->getfield(2);
	modify = (unsigned char*)(rec + f->getoffset());
	f = tab->getfield(5);
	start = (int32_t*)(rec + f->getoffset());
	length = start + 1;

	if(tab->get_numfields() > 6) partno = tab->getfield(6);
	else partno = NULL;

	for(i = 0; i < tab->get_phys_numrecords(); ++i)
	{
		tab->getrecord(i, rec);
		if(*rec) continue;
		if(*start == 0) continue;
		if(*length == 0) continue;

		tr.name = filename->get_presentation(rec);
		if(tr.name.IsEmpty()) continue;

		tr.addr.blob_start = *start;
		tr.addr.blob_length = *length;

		if(partno) tr.partno = partno->get_presentation(rec, true).ToIntDef(0);
		else tr.partno = 0;
		time1CD_to_FileTime(&tr.ft_create, create);
		time1CD_to_FileTime(&tr.ft_modify, modify);

		allrec.push_back(tr);

		s = tr.name.UpperCase();
		pmaxpartno = maxpartnos.find(s);
		if(pmaxpartno == maxpartnos.end()) maxpartnos[s] = tr.partno;
		else if(pmaxpartno->second < tr.partno) pmaxpartno->second = tr.partno;
	}

	for(pmaxpartno = maxpartnos.begin(); pmaxpartno != maxpartnos.end(); ++pmaxpartno)
	{
		tf = new table_file(tab, pmaxpartno->first, pmaxpartno->second);
		allfiles[pmaxpartno->first] = tf;
	}

	for(j = 0; j < allrec.size(); ++j)
	{
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
	if (rec) {
		delete[] rec;
		rec = nullptr;
	}
	for (auto p : allfiles) {
		delete p.second;
	}
	allfiles.clear();
}

//---------------------------------------------------------------------------
bool TableFiles::test_table()
{
	if(!tab) return false;
	if(tab->get_numfields() < 6)
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. В таблице меньше 6 полей"
			,"Таблица", tab->getname()
			,"Кол-во полей", tab->get_numfields());
		return false;
	}

	if(tab->get_numfields() > 7)
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. В таблице больше 7 полей"
			,"Таблица", tab->getname()
			,"Кол-во полей", tab->get_numfields());
		return false;
	}

	if(tab->getfield(0)->getname().CompareIC("FILENAME"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Первое поле таблицы не FILENAME"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(0)->getname());
		return false;
	}

	if(tab->getfield(1)->getname().CompareIC("CREATION"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Второе поле таблицы не CREATION"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(1)->getname());
		return false;
	}

	if(tab->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Третье поле таблицы не MODIFIED"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(2)->getname());
		return false;
	}

	if(tab->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Четвертое поле таблицы не ATTRIBUTES"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(3)->getname());
		return false;
	}

	if(tab->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Пятое поле таблицы не DATASIZE"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(4)->getname());
		return false;
	}

	if(tab->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Шестое поле таблицы не BINARYDATA"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(5)->getname());
		return false;
	}

	if(tab->get_numfields() > 6)
	{
		if(tab->getfield(6)->getname().CompareIC("PARTNO"))
		{
			msreg_g.AddError("Ошибка проверки таблицы контейнера файлов. Седьмое поле таблицы не PARTNO"
				,"Таблица", tab->getname()
				,"Поле", tab->getfield(6)->getname());
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
table_file* TableFiles::getfile(const String& name)
{
	std::map<String,table_file*>::iterator p;

	p = allfiles.find(name.UpperCase());
	if(p == allfiles.end()) return NULL;
	else return p->second;
}

std::map<String,table_file*> &TableFiles::files()
{
	return allfiles;
}
