
#include <getcfname_main.h>
#include <System.IOUtils.hpp>
#include "Zip.h"

MessageRegistrator* msreg;
const int TEMP_BUFFER_SIZE = 1024;
char temp[TEMP_BUFFER_SIZE];

//---------------------------------------------------------------------------
void get_cf_name(T_1CD* base, Messager* mess);
void get_cf_name(tree* tr, Messager* mess);
void get_cf_name(v8catalog* cf, Messager* mess);

//---------------------------------------------------------------------------
char* oem(const char* str)
{
	CharToOemA(str, temp);
	return temp;
}

//---------------------------------------------------------------------------
char* oem(const String& str)
{
	WideCharToMultiByte(CP_OEMCP, WC_NO_BEST_FIT_CHARS, str.c_str(), -1, temp, TEMP_BUFFER_SIZE - 1, "?", NULL);
	return temp;
}
//---------------------------------------------------------------------------
__fastcall Messager::Messager()
{
	has_error = false;
}

//---------------------------------------------------------------------------
void __fastcall Messager::Status(const String& message)
{
}

//---------------------------------------------------------------------------
void __fastcall Messager::AddMessage(const String& message, const MessageState mstate, TStringList* param)
{
	int i;

	if(mstate == msWarning || mstate == msError) has_error = true;
	switch(mstate)
	{
		case msEmpty: break;
		case msSuccesfull: break;
		case msWarning: cout << "<Warning> "; break;
		case msInfo: cout << "<Info> "; break;
		case msError: cout << "<Error> "; break;
		case msWait: cout << "<Wait> "; break;
		case msHint: cout << "<Hint> "; break;
	}
	cout << oem(message) << "\n";
	if(param) for(i = 1; i <= param->Count; i++) cout << "\t" << oem((*param)[i]) << "\n";
}

//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	Messager* mess; // регистратор сообщений
	T_1CD* base1CD; // база 1CD
	v8catalog* cf; // конфигурация
	int ret_value;
	String filename;
	String extension;

	mess = new Messager;
	msreg = mess;

	if(argc <= 1)
	{
		mess->AddMessage(L"getcfname: Определение имени и версии конфигурации файловых баз *.1cd и файлов конфигураций *.cf\n(c)awa 2012-2016\n\nИспользование:\ngetcfname.exe <1CD-file>\ngetcfname.exe <CF-file>", msEmpty);
		delete mess;
		return 2;
	}

	if(argc > 2)
	{
		mess->AddError(L"Слишком много параметров\n\nИспользование:\ngetcfname.exe <1CD-file>\ngetcfname.exe <CF-file>");
		delete mess;
		return 2;
	}

	filename = String(argv[1]);
	extension = TPath::GetExtension(filename);
	if(extension.CompareIC(L".1cd") == 0)
	{
		base1CD = new T_1CD(filename, mess, false);
		get_cf_name(base1CD, mess);
		delete base1CD;
	}
	else if(extension.CompareIC(L".cf") == 0)
	{
		cf = new v8catalog(filename, true);
		get_cf_name(cf, mess);
		delete cf;
	}
	else
	{
		mess->AddError(L"Неподдерживаемое расширение файла\n\nИспользование:\ngetcfname.exe <1CD-file>\ngetcfname.exe <CF-file>");
		delete mess;
		return 2;
	}

	ret_value = mess->has_error ? 1 : 0;
	delete mess;
	return ret_value;
}

tree* get_tree_from_record(table* t, String recname, Messager* mess)
{
	unsigned int i;
	int j;
	char* rec;
	field* f;
	field* fd;
	bool ok;
	TMemoryStream* st_pack;
	TBytesStream* st_unpack;
	char* bynd;
	tree* tr;
	TEncoding *enc;
	TBytes bytes;
	String s;
	int offset;

	f = t->getfield(0);
	fd = t->getfield(5);
	rec = new char[t->get_recordlen()];
	ok = false;
	for(i = 0; i < t->get_phys_numrecords(); i++)
	{
		t->getrecord(i, rec);
		if(*rec) continue;
		if(f->get_presentation(rec).CompareIC(recname) == 0)
		{
			ok = true;
			break;
		}
	}

	if(!ok)
	{
		s = L"Не найдена запись ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
		delete[] rec;
		return NULL;
	}

	st_pack = new TMemoryStream;
	st_unpack = new TBytesStream(bytes);

	bynd = rec + fd->getoffset();
	t->readBlob(st_pack, *(unsigned int*)bynd, *(unsigned int*)(bynd + 4));
	delete[] rec;

	try
	{
		st_pack->Seek(0, soFromBeginning);
		InflateStream(st_pack, st_unpack);
	}
	catch(...)
	{
		s = L"Ошибка распаковки записи ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
		delete[] rec;
		delete st_pack;
		delete st_unpack;
		return NULL;
	}
	delete st_pack;

	enc = NULL;
	offset = TEncoding::GetBufferEncoding(st_unpack->Bytes, enc);
	if(offset == 0)
	{
		s = L"Ошибка определения кодировки записи ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
		delete st_unpack;
		return NULL;
	}
	bytes = TEncoding::Convert(enc, TEncoding::Unicode, st_unpack->Bytes, offset, st_unpack->Size-offset);

	s = t->getname();
	s += L":";
	s += recname;
	tr = parse_1Ctext(String((wchar_t*)&bytes[0], bytes.Length / 2), s);

	delete st_unpack;

	if(!tr)
	{
		s = L"Ошибка разбора записи ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
	}

	return tr;

}

v8catalog* get_v8catalog_from_record(table* t, String recname, Messager* mess)
{
	unsigned int i;
	int j;
	char* rec;
	field* f;
	field* fd;
	bool ok;
	TMemoryStream* st_pack;
	TMemoryStream* st_unpack;
	char* bynd;
	String s;
	v8catalog* cat;

	f = t->getfield(0);
	fd = t->getfield(5);
	rec = new char[t->get_recordlen()];
	ok = false;
	for(i = 0; i < t->get_phys_numrecords(); i++)
	{
		t->getrecord(i, rec);
		if(*rec) continue;
		if(f->get_presentation(rec).CompareIC(recname) == 0)
		{
			ok = true;
			break;
		}
	}

	if(!ok)
	{
		s = L"Не найдена запись ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
		delete[] rec;
		return NULL;
	}

	st_pack = new TMemoryStream;
	st_unpack = new TMemoryStream;

	bynd = rec + fd->getoffset();
	t->readBlob(st_pack, *(unsigned int*)bynd, *(unsigned int*)(bynd + 4));
	delete[] rec;

	try
	{
		st_pack->Seek(0, soFromBeginning);
		InflateStream(st_pack, st_unpack);
	}
	catch(...)
	{
		s = L"Ошибка распаковки записи ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
		delete[] rec;
		delete st_pack;
		delete st_unpack;
		return NULL;
	}
	delete st_pack;

	cat = new v8catalog(st_unpack, false);

	delete st_unpack;

	if(!cat)
	{
		s = L"Ошибка разбора записи ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
	}

	return cat;

}

void get_cf_name(tree* tr, Messager* mess)
{
	int j, k;
	tree* node;
	tree* node2;
	String cf_synonym;
	String cf_version;
	String s;
	bool ok;
	int structver;
	int offset_name;
	int offset_ver;

	node = tr;
	node = &(*node)[0][3][1][1];

	structver = (*node)[0].get_value().ToInt();

//	switch(structver)
//	{
//		case 15:
//			offset_name = 1;
//			offset_ver = 12;
//			break;
//		case 22:
//		case 32:
//		case 34:
//		case 36:
//		case 37:
//		case 46:
//			offset_name = 1;
//			offset_ver = 15;
//			break;
//		default:
//			s = L"Неизвестная версия ";
//			s += structver;
//			s += L" блока параметров конфигурации";
//			mess->AddError(s);
//			delete tr;
//			return;
//	}

	if(structver <= 15)
	{
		offset_name = 1;
		offset_ver = 12;
	}
	else
	{
		offset_name = 1;
		offset_ver = 15;
	}

	node2 = &(*node)[offset_name][1][3];

	if(node2->get_type() != nd_list)
	{
		mess->AddError(L"Ошибка получения синонима имени конфигурации");
		delete tr;
		return;
	}
	if((*node2)[0].get_type() != nd_number)
	{
		mess->AddError(L"Ошибка получения синонима имени конфигурации");
		delete tr;
		return;
	}

	j = (*node2)[0].get_value().ToIntDef(0);
	if(j == 0) cf_synonym = (*node)[offset_name][1][2].get_value();
	else
	{
		ok = false;
		for(k = 0; k < j; k++)
		{
			if((*node2)[k * 2 + 1].get_value().CompareIC(L"ru") == 0)
			{
				cf_synonym = (*node2)[k * 2 + 2].get_value();
				ok = true;
				break;
			}
		}

		if(!ok) cf_synonym = (*node2)[2].get_value();
	}

	cf_version = (*node)[offset_ver].get_value();
	delete tr;

	mess->AddMessage(cf_synonym + " (" + cf_version + ")", msEmpty);

}

void get_cf_name(T_1CD* base, Messager* mess)
{
	table* t;
	tree* tr;
	tree* node;
	v8catalog* cat;
	v8file* f;
	String meta;
	String s;
	int ver;

	if(!base) return;
	if(!base->is_open()) return;
	t = base->table_config;

	if(!t) return;

	if(t->get_numfields() < 6)
	{
		mess->AddError(L"В таблице CONFIG меньше 6 полей",
			L"Кол-во полей", t->get_numfields());
		return;
	}

	if(t->get_numfields() > 7)
	{
		mess->AddError(L"В таблице CONFIG больше 7 полей",
			L"Кол-во полей", t->get_numfields());
		return;
	}

	if(t->getfield(0)->getname().CompareIC(L"FILENAME"))
	{
		mess->AddError(L"Первое поле таблицы CONFIG не FILENAME",
			L"Поле", t->getfield(0)->getname());
		return;
	}

	if(t->getfield(1)->getname().CompareIC(L"CREATION"))
	{
		mess->AddError(L"Второе поле таблицы CONFIG не CREATION",
			L"Поле", t->getfield(1)->getname());
		return;
	}

	if(t->getfield(2)->getname().CompareIC(L"MODIFIED"))
	{
		mess->AddError(L"Третье поле таблицы CONFIG не MODIFIED",
			L"Поле", t->getfield(2)->getname());
		return;
	}

	if(t->getfield(3)->getname().CompareIC(L"ATTRIBUTES"))
	{
		mess->AddError(L"Четвертое поле таблицы CONFIG не ATTRIBUTES",
			L"Поле", t->getfield(3)->getname());
		return;
	}

	if(t->getfield(4)->getname().CompareIC(L"DATASIZE"))
	{
		mess->AddError(L"Пятое поле таблицы CONFIG не DATASIZE",
			L"Поле", t->getfield(4)->getname());
		return;
	}

	if(t->getfield(5)->getname().CompareIC(L"BINARYDATA"))
	{
		mess->AddError(L"Шестое поле таблицы CONFIG не BINARYDATA",
			L"Поле", t->getfield(5)->getname());
		return;
	}

	tr = get_tree_from_record(t, L"version", mess);
	if(!tr) return;
	node = tr;

	node = &(*node)[0][0][0];
	if(node->get_type() != nd_number)
	{
		mess->AddError(L"Ошибка получения версии формата конфигурации");
		delete tr;
		return;
	}

	ver = node->get_value().ToInt();
	delete tr;

	if(ver < 100)
	{
		// 8.0
		cat = get_v8catalog_from_record(t, L"metadata", mess);
		if(!cat) return;

		f = cat->GetFile(L"root");
		if(!f)
		{
			mess->AddError(L"Ошибка получения файла root из записи metadata таблицы CONFIG");
			delete cat;
			return;
		}

		tr = get_treeFromV8file(f);
		if(!tr)
		{
			mess->AddError(L"Ошибка разбора файла root из записи metadata таблицы CONFIG");
			delete cat;
			return;
		}

		node = tr;
		node = &(*node)[0][1];

		if(node->get_type() != nd_guid)
		{
			mess->AddError(L"Ошибка получения имени файла метаданных");
			delete cat;
			delete tr;
			return;
		}
		meta = node->get_value();
		delete tr;

		f = cat->GetFile(meta);
		if(!f)
		{
			s = L"Ошибка получения файла ";
			s += meta;
			s += L" из записи metadata таблицы CONFIG";
			mess->AddError(s);
			delete cat;
			return;
		}

		tr = get_treeFromV8file(f);
		if(!tr)
		{
			s = L"Ошибка разбора файла ";
			s += meta;
			s += L" из записи metadata таблицы CONFIG";
			mess->AddError(s);
			delete cat;
			return;
		}
	}
	else
	{
		//8.1 или 8.2
		tr = get_tree_from_record(t, L"root", mess);
		if(!tr) return;
		node = tr;

		node = &(*node)[0][1];

		if(node->get_type() != nd_guid)
		{
			mess->AddError(L"Ошибка получения имени файла метаданных");
			delete tr;
			return;
		}
		meta = node->get_value();
		delete tr;

		tr = get_tree_from_record(t, meta, mess);
		if(!tr) return;
	}

	get_cf_name(tr, mess);
}

void get_cf_name(v8catalog* cf, Messager* mess)
{
	tree* tr;
	tree* node;
	v8catalog* cat;
	v8file* f;
	String meta;
	String s;
	int ver;

	if(!cf)
	{
		mess->AddError(L"Ошибка открытия файла");
		return;
	}

	f = cf->GetFile(L"version");
	if(!f)
	{
		mess->AddError(L"Ошибка получения файла root конфигурации");
		return;
	}

	tr = get_treeFromV8file(f);
	if(!tr)
	{
		mess->AddError(L"Ошибка разбора файла root конфигурации");
		return;
	}

	tr = get_treeFromV8file(f);
	if(!tr) return;
	node = tr;

	node = &(*node)[0][0][0];
	if(node->get_type() != nd_number)
	{
		mess->AddError(L"Ошибка получения версии формата конфигурации");
		delete tr;
		return;
	}

	ver = node->get_value().ToInt();
	delete tr;

	if(ver < 100)
	{
		// 8.0
		f = cf->GetFile(L"metadata");
		if(!f)
		{
			mess->AddError(L"Ошибка получения файла metadata конфигурации");
			return;
		}

		cat = f->GetCatalog();
		if(!cat)
		{
			mess->AddError(L"Ошибка открытия файла metadata конфигурации");
			return;
		}

	}
	else
	{
		//8.1 или 8.2
		cat = cf;
	}

	f = cat->GetFile(L"root");
	if(!f)
	{
		mess->AddError(L"Ошибка получения файла root конфигурации");
		return;
	}

	tr = get_treeFromV8file(f);
	if(!tr)
	{
		mess->AddError(L"Ошибка разбора файла root конфигурации");
		return;
	}

	node = tr;
	node = &(*node)[0][1];

	if(node->get_type() != nd_guid)
	{
		mess->AddError(L"Ошибка получения имени файла метаданных");
		delete tr;
		return;
	}

	meta = node->get_value();
	delete tr;

	f = cat->GetFile(meta);
	if(!f)
	{
		s = L"Ошибка получения файла ";
		s += meta;
		s += L" конфигурации";
		mess->AddError(s);
		return;
	}

	tr = get_treeFromV8file(f);
	if(!tr)
	{
		s = L"Ошибка разбора файла ";
		s += meta;
		s += L" конфигурации";
		mess->AddError(s);
		return;
	}

	get_cf_name(tr, mess);
}


