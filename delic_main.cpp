
#include <delic_main.h>
#include <System.IOUtils.hpp>
#include "Zip.h"

MessageRegistrator* msreg;
const int TEMP_BUFFER_SIZE = 1024;
char temp[TEMP_BUFFER_SIZE];
String rn(L"\r\n");
String emptyuid(L"00000000-0000-0000-0000-000000000000");

//---------------------------------------------------------------------------
void __fastcall cure(T_1CD* base, Messager* mess);

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
	if(param) for(i = 0; i < param->Count; ++i) cout << "\t" << oem((*param)[i]) << "\n";
}

//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	Messager* mess; // регистратор сообщений
	T_1CD* base1CD; // база 1CD
	//v8catalog* cf; // конфигурация
	int ret_value;
	String filename;
	String extension;

	mess = new Messager;
	msreg = mess;

	if(argc <= 1)
	{
		mess->AddMessage(L"delic: Лечение больных жадностью файловых баз *.1cd\n(c)*** 2016,2017\n\nИспользование:\ndelic.exe <1CD-file>", msEmpty);
		delete mess;
		return 2;
	}

	if(argc > 2)
	{
		mess->AddError(L"Слишком много параметров\n\nИспользование:\ndelic.exe <1CD-file>");
		delete mess;
		return 2;
	}

	filename = String(argv[1]);
	extension = TPath::GetExtension(filename);
	if(extension.CompareIC(L".1cd") == 0)
	{
		base1CD = new T_1CD(filename, mess, true);
		if(base1CD->get_readonly())
		{
			mess->AddError(L"База открыта в режиме только для чтения. Изменения невозможны.");
			return 3;
		}
		cure(base1CD, mess);
		delete base1CD;
	}
	else
	{
		mess->AddError(L"Неподдерживаемое расширение файла\n\nИспользование:\ndelic.exe <1CD-file>");
		delete mess;
		return 2;
	}

	ret_value = mess->has_error ? 1 : 0;
	delete mess;
	return ret_value;
}

tree* __fastcall get_tree_from_record(table* t, unsigned int physnum, Messager* mess, String& recordtext, bool packed = false)
{
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
	String recname;
	int offset;

	f = t->getfield(0);
	fd = t->getfield(5);
	rec = new char[t->get_recordlen()];
	t->getrecord(physnum, rec);
	recname = f->get_presentation(rec);

	st_unpack = new TBytesStream(bytes);
	bynd = rec + fd->getoffset();
	if(packed)
	{
		TMemoryStream* st_pack = new TMemoryStream;
		t->readBlob(st_pack, *(unsigned int*)bynd, *(unsigned int*)(bynd + 4));
		st_pack->Seek(0, soFromBeginning);
		try
		{
			InflateStream(st_pack, st_unpack);
		}
		catch (...)
		{
			s = L"Ошибка распаковки файла ";
			s += recname;
			s += L" в таблице ";
			s += t->getname();
			mess->AddError(s);
			delete st_pack;
			delete st_unpack;
			return NULL;
		}
		delete st_pack;
	}
	else
	{
		t->readBlob(st_unpack, *(unsigned int*)bynd, *(unsigned int*)(bynd + 4));
	}
	delete[] rec;

	enc = NULL;
	offset = TEncoding::GetBufferEncoding(st_unpack->Bytes, enc);
	if(offset == 0)
	{
		s = L"Ошибка определения кодировки файла ";
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
	recordtext = String((wchar_t*)&bytes[0], bytes.Length / 2);
	tr = parse_1Ctext(recordtext, s);

	delete st_unpack;

	if(!tr)
	{
		s = L"Ошибка разбора файла ";
		s += recname;
		s += L" в таблице ";
		s += t->getname();
		mess->AddError(s);
	}

	return tr;

}

bool __fastcall set_tree_to_record(table* t, unsigned int physnum, tree* tr, Messager* mess, bool packed = false)
{
	String s;
	TMemoryStream* ms;
	TMemoryStream* ms2;
	TStreamWriter* sw;

	s = outtext(tr);

	ms = new TMemoryStream;
	if(packed)
	{
		ms2 = new TMemoryStream;
		ms2->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
		sw = new TStreamWriter(ms2, TEncoding::UTF8, 1024);
		sw->Write(s);
		delete sw;
		ms2->Seek(0, soFromBeginning);
		DeflateStream(ms2, ms);
		delete ms2;
	}
	else
	{
		ms->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
		sw = new TStreamWriter(ms, TEncoding::UTF8, 1024);
		sw->Write(s);
		delete sw;
	}

	t->set_edit_value(physnum, 5, false, L"", ms);
	t->set_edit_value(physnum, 4, false, ms->Size);

	//delete ms; Это не нужно делать, поток удалит потом таблица при окончании редактирования
	return true;
}


void __fastcall cure(T_1CD* base, Messager* mess)
{
	table* t;
	table* tparams;
	table* tfiles;
	table* tconfig;
	tree* tr;
	tree* node;
	tree* snode;
	tree* snode2;
	unsigned int i;
	char* rec;
	field* f;
	String fname;
	String recordtext;
	bool ok;
	bool changedparams;
	bool changedfiles;
	bool changedconfig;
	bool changed;
	TMemoryStream* ms;

	if(!base) return;
	if(!base->is_open()) return;
	t = base->table_params;
	tparams = t;

	if(!t) return;

	if(t->get_numfields() < 6)
	{
		mess->AddError(L"В таблице PARAMS меньше 6 полей",
			L"Кол-во полей", t->get_numfields());
		return;
	}

	if(t->get_numfields() > 7)
	{
		mess->AddError(L"В таблице PARAMS больше 7 полей",
			L"Кол-во полей", t->get_numfields());
		return;
	}

	if(t->getfield(0)->getname().CompareIC(L"FILENAME"))
	{
		mess->AddError(L"Первое поле таблицы PARAMS не FILENAME",
			L"Поле", t->getfield(0)->getname());
		return;
	}

	if(t->getfield(1)->getname().CompareIC(L"CREATION"))
	{
		mess->AddError(L"Второе поле таблицы PARAMS не CREATION",
			L"Поле", t->getfield(1)->getname());
		return;
	}

	if(t->getfield(2)->getname().CompareIC(L"MODIFIED"))
	{
		mess->AddError(L"Третье поле таблицы PARAMS не MODIFIED",
			L"Поле", t->getfield(2)->getname());
		return;
	}

	if(t->getfield(3)->getname().CompareIC(L"ATTRIBUTES"))
	{
		mess->AddError(L"Четвертое поле таблицы PARAMS не ATTRIBUTES",
			L"Поле", t->getfield(3)->getname());
		return;
	}

	if(t->getfield(4)->getname().CompareIC(L"DATASIZE"))
	{
		mess->AddError(L"Пятое поле таблицы PARAMS не DATASIZE",
			L"Поле", t->getfield(4)->getname());
		return;
	}

	if(t->getfield(5)->getname().CompareIC(L"BINARYDATA"))
	{
		mess->AddError(L"Шестое поле таблицы PARAMS не BINARYDATA",
			L"Поле", t->getfield(5)->getname());
		return;
	}

	f = t->getfield(0); // Поле с именем (FILENAME)
	rec = new char[t->get_recordlen()];
	t->begin_edit();
	ok = true;
	changed = false;

	for(i = 0; i < t->get_phys_numrecords(); ++i)
	{
		t->getrecord(i, rec);
		if(*rec) continue;
		fname = f->get_presentation(rec);
		if(fname.CompareIC(L"ibparams.inf") == 0)
		{
			mess->AddMessage(L"Правим файл ibparams.inf", msEmpty);

			tr = get_tree_from_record(t, i, mess, recordtext);
			if(!tr)
			{
				ok = false;
				break;
			}
			node = &(*tr)[0];
			if(node->get_num_subnode() <= 8) mess->AddMessage(L"Файл ibparams.inf править не понадобилось", msEmpty);
			else if(node->get_num_subnode() == 9 || node->get_num_subnode() == 10)
			{
				snode = node->get_subnode(7);
				snode2 = node->get_subnode(8);
				if(snode->get_type() != nd_guid || snode2->get_type() != nd_guid)
				{
					snode = node->get_subnode(9);
					if(snode->get_type() != nd_guid || snode2->get_type() != nd_guid)
					{
						mess->AddError(L"Формат файл ibparams.inf не совпадает с ожидаемым. Правка невозможна"
						, L"Содержание ibparams.inf", rn + recordtext);
						ok = false;
						delete tr;
						break;
					}
				}

				if(snode->get_value().CompareIC(emptyuid) == 0 && snode->get_value().CompareIC(emptyuid) == 0)
				{
					mess->AddMessage(L"Файл ibparams.inf править не понадобилось", msEmpty);
				}
				else
				{
					snode->set_value(emptyuid, nd_guid);
					snode2->set_value(emptyuid, nd_guid);

					if(set_tree_to_record(t, i, tr, mess)) changed = true;
					else
					{
						mess->AddError(L"Не удалось записать файл ibparams.inf");
						ok = false;
						delete tr;
						break;
					}
				}
			}
			else
			{
				mess->AddError(L"Формат файл ibparams.inf не совпадает с ожидаемым. Правка невозможна"
					, L"Содержание ibparams.inf", rn + recordtext);
				ok = false;
				delete tr;
				break;
			}

			delete tr;

		}
		else if(fname.SubString(fname.Length() - 2, 3).CompareIC(L".ui") == 0)
		{
			mess->AddMessage(String(L"Удаляем файл ") + fname, msEmpty);
			t->delete_record(i);
			changed = true;
		}
		else if(fname.CompareIC(L"locale.inf") == 0)
		{
			mess->AddMessage(L"Правим файл locale.inf", msEmpty);

			tr = get_tree_from_record(t, i, mess, recordtext);
			if(!tr)
			{
				ok = false;
				break;
			}
			node = &(*tr)[0];
			if(node->get_num_subnode() <= 11) mess->AddMessage(L"Файл locale.inf править не понадобилось", msEmpty);
			else if(node->get_num_subnode() == 13)
			{
				snode = node->get_subnode(11);
				snode2 = node->get_subnode(12);
				if(snode->get_type() != nd_guid || snode2->get_type() != nd_guid)
				{
					mess->AddError(L"Формат файл locale.inf не совпадает с ожидаемым. Правка невозможна"
					, L"Содержание locale.inf", rn + recordtext);
					ok = false;
					delete tr;
					break;
				}

				if(snode->get_value().CompareIC(emptyuid) == 0 && snode->get_value().CompareIC(emptyuid) == 0)
				{
					mess->AddMessage(L"Файл locale.inf править не понадобилось", msEmpty);
				}
				else
				{
					snode->set_value(emptyuid, nd_guid);
					snode2->set_value(emptyuid, nd_guid);

					if(set_tree_to_record(t, i, tr, mess)) changed = true;
					else
					{
						mess->AddError(L"Не удалось записать файл locale.inf");
						ok = false;
						delete tr;
						break;
					}
				}
			}
			else
			{
				mess->AddError(L"Формат файл locale.inf не совпадает с ожидаемым. Правка невозможна"
					, L"Содержание locale.inf", rn + recordtext);
				ok = false;
				delete tr;
				break;
			}

			delete tr;

		}
		else if(fname.CompareIC(L"log.inf") == 0)
		{
			mess->AddMessage(L"Правим файл log.inf", msEmpty);

			tr = get_tree_from_record(t, i, mess, recordtext);
			if(!tr)
			{
				ok = false;
				break;
			}
			node = &(*tr)[0];
			if(node->get_num_subnode() <= 3) mess->AddMessage(L"Файл log.inf править не понадобилось", msEmpty);
			else if(node->get_num_subnode() == 5)
			{
				snode = node->get_subnode(3);
				snode2 = node->get_subnode(4);
				if(snode->get_type() != nd_guid || snode2->get_type() != nd_guid)
				{
					mess->AddError(L"Формат файл log.inf не совпадает с ожидаемым. Правка невозможна"
					, L"Содержание log.inf", rn + recordtext);
					ok = false;
					delete tr;
					break;
				}

				if(snode->get_value().CompareIC(emptyuid) == 0 && snode->get_value().CompareIC(emptyuid) == 0)
				{
					mess->AddMessage(L"Файл log.inf править не понадобилось", msEmpty);
				}
				else
				{
					snode->set_value(emptyuid, nd_guid);
					snode2->set_value(emptyuid, nd_guid);

					if(set_tree_to_record(t, i, tr, mess)) changed = true;
					else
					{
						mess->AddError(L"Не удалось записать файл log.inf");
						ok = false;
						delete tr;
						break;
					}
				}
			}
			else
			{
				mess->AddError(L"Формат файл log.inf не совпадает с ожидаемым. Правка невозможна"
					, L"Содержание log.inf", rn + recordtext);
				ok = false;
				delete tr;
				break;
			}

			delete tr;

		}
	}

	changedparams = changed;
	changed = false;

#ifdef delicfiles
	if(ok)
	{
		t = base->table_files;
		tfiles = t;

		if(!t) return;

		if(t->get_numfields() < 6)
		{
			mess->AddError(L"В таблице FILES меньше 6 полей",
				L"Кол-во полей", t->get_numfields());
			return;
		}

		if(t->get_numfields() > 7)
		{
			mess->AddError(L"В таблице FILES больше 7 полей",
				L"Кол-во полей", t->get_numfields());
			return;
		}

		if(t->getfield(0)->getname().CompareIC(L"FILENAME"))
		{
			mess->AddError(L"Первое поле таблицы FILES не FILENAME",
				L"Поле", t->getfield(0)->getname());
			return;
		}

		if(t->getfield(1)->getname().CompareIC(L"CREATION"))
		{
			mess->AddError(L"Второе поле таблицы FILES не CREATION",
				L"Поле", t->getfield(1)->getname());
			return;
		}

		if(t->getfield(2)->getname().CompareIC(L"MODIFIED"))
		{
			mess->AddError(L"Третье поле таблицы FILES не MODIFIED",
				L"Поле", t->getfield(2)->getname());
			return;
		}

		if(t->getfield(3)->getname().CompareIC(L"ATTRIBUTES"))
		{
			mess->AddError(L"Четвертое поле таблицы FILES не ATTRIBUTES",
				L"Поле", t->getfield(3)->getname());
			return;
		}

		if(t->getfield(4)->getname().CompareIC(L"DATASIZE"))
		{
			mess->AddError(L"Пятое поле таблицы FILES не DATASIZE",
				L"Поле", t->getfield(4)->getname());
			return;
		}

		if(t->getfield(5)->getname().CompareIC(L"BINARYDATA"))
		{
			mess->AddError(L"Шестое поле таблицы PARAMS не BINARYDATA",
				L"Поле", t->getfield(5)->getname());
			return;
		}

		f = t->getfield(0); // Поле с именем (FILENAME)
		rec = new char[t->get_recordlen()];
		t->begin_edit();

		for(i = 0; i < t->get_phys_numrecords(); ++i)
		{
			t->getrecord(i, rec);
			if(*rec) continue;
			fname = f->get_presentation(rec);
			if(fname.CompareIC(L"c01b78f6-1525-41b1-9cc1-69e3da58d2ac.pfl") == 0)
			{
				if(*(int*)&(rec[t->getfield(5)->getoffset() + 4]) == 0)
				//if(*(int*)&(rec + t->getfield(5)->getoffset() + 4) == 0)
				{
					mess->AddMessage(L"Файл c01b78f6-1525-41b1-9cc1-69e3da58d2ac.pfl править не понадобилось", msEmpty);
				}
				else
				{
					ms = new TMemoryStream;
					t->set_edit_value(i, 5, false, L"", ms);
					t->set_edit_value(i, 4, false, ms->Size);
					mess->AddMessage(String(L"Очищаем файл ") + fname, msEmpty);
					changed = true;
				}
			}
		}

		changedfiles = changed;
		changed = false;

		t = base->table_config;
		tconfig = t;

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
			mess->AddError(L"Шестое поле таблицы PARAMS не BINARYDATA",
				L"Поле", t->getfield(5)->getname());
			return;
		}

		f = t->getfield(0); // Поле с именем (FILENAME)
		rec = new char[t->get_recordlen()];
		t->begin_edit();

		for(i = 0; i < t->get_phys_numrecords(); ++i)
		{
			t->getrecord(i, rec);
			if(*rec) continue;
			fname = f->get_presentation(rec);
			if(fname.CompareIC(L"root") == 0)
			{
				mess->AddMessage(L"Правим файл root", msEmpty);

				tr = get_tree_from_record(t, i, mess, recordtext, true);
				if(!tr)
				{
					//ok = false;
					break;
				}
				node = &(*tr)[0];
				if(node->get_num_subnode() <= 2) mess->AddMessage(L"Файл root править не понадобилось", msEmpty);
				else
				{
					snode = node->get_subnode(0);
					if(snode->get_value().CompareIC(L"2") == 0)
					{
						snode2 = node->get_subnode(2);
						if(snode2->get_value().CompareIC(L""))
						{
							if(snode2->get_type() != nd_binary2)
							{
								mess->AddError(L"Формат файл root не совпадает с ожидаемым. Правка невозможна"
								, L"Содержание root", rn + recordtext);
								//ok = false;
								delete tr;
								break;
							}
							snode2->set_value(L"", nd_binary2);
							if(set_tree_to_record(t, i, tr, mess, true)) changed = true;
							else
							{
								mess->AddError(L"Не удалось записать файл root");
								//ok = false;
								delete tr;
								break;
							}
						}
						else
						{
							mess->AddMessage(L"Файл root править не понадобилось", msEmpty);
						}

					}
					else
					{
						String s = L"Неизвестная версия \"";
						s += snode->get_value();
						s += "\" файла root. Файл root не изменен.";
						mess->AddError(s);
						//ok = false;
						delete tr;
						break;
					}
				}
				delete tr;
			}
		}
		changedconfig = changed;
		//changed = false;
	}
#endif


	if(ok)
	{
		changed = changedparams || changedfiles || changedconfig;

		if(changed)
		{
			if(changedparams) tparams->end_edit();
			if(changedfiles) tfiles->end_edit();
			if(changedconfig) tconfig->end_edit();
			mess->AddMessage(L"База изменена", msSuccesfull);
		}
		else
		{
			t->cancel_edit();
			mess->AddMessage(L"Изменять базу не потребовалось", msSuccesfull);
		}
	}
	else
	{
		t->cancel_edit();
		mess->AddError(L"Не удалось вылечить базу. Изменения не сохранены");
	}

}


